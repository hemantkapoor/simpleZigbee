/*
 * Observer.cpp
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */
#include <sstream>
#include <vector>
#include <algorithm>
#include "../../simpleDebug/SimpleDebug.h"
#include "../../simpleSerial/utility/Utility.h"
#include "../object/BaseObject.h"
#include "Observer.h"

using namespace SimpleZigbeeName;

Observer::Observer(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
	m_debug = SimpleDebugName::SimpleDebug::instance();
	m_stopThread = false;
	m_thread = std::make_unique<std::thread>(&Observer::asynchronousThread, this);
}

void Observer::handleReceivedMessage(std::unique_ptr<BaseObject> messageObject)
{
	if(!messageObject)
	{
		m_debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : No Object present\r\n");
		return;
	}
	auto command = messageObject->getCommand();
	{
		std::lock_guard<std::mutex> guard(m_syncRequestResponseListMutex);
		//Check if we have are waiting for this message in a synchronous event
		auto listIter = std::find(m_syncRequestResponseList.begin(), m_syncRequestResponseList.end(), command);
		if(listIter != m_syncRequestResponseList.end())
		{
			std::lock_guard<std::mutex> guard(m_syncResponseMapMutex);
			m_syncResponseMap[command] = std::move(messageObject);
			std::stringstream outputSting;
			outputSting <<__PRETTY_FUNCTION__<< " : Added Command Id 0x"<< std::hex << int(command) << std::endl;
			m_debug->log(SimpleDebugName::LOG, outputSting);
			m_dataAvailable = true;
			m_condVar.notify_one();
			return;
		}
	}

	//If here then we will check if the command is registered for asynchronous call back
	{
	std::lock_guard<std::mutex> guard(m_registeredCommandsMapMutex);
		if(m_registeredCommandsMap.find(command) == m_registeredCommandsMap.end())
		{
			std::stringstream outputSting;
			outputSting <<  std::string(__PRETTY_FUNCTION__) + " : 0x"<< std::hex << command << " Command not registered " << std::endl;
			m_debug->log(SimpleDebugName::CRITICAL_WARNING, outputSting);
			return;
		}
	}
	//We will add this to the list and notify our sleeping thread.
	std::lock_guard<std::mutex> guard(m_receivedCommandObjectsMutex);
	m_receivedCommandObjects.push_back(std::move(messageObject));
	m_asynchronousDataAvailable = true;
	m_asynchronousCondVar.notify_one();
}

void Observer::requestSyncResponse(uint16_t command)
{
	std::lock_guard<std::mutex> guard(m_syncRequestResponseListMutex);
	m_syncRequestResponseList.push_back(command);
}
std::unique_ptr<BaseObject> Observer::getSyncResponse(uint16_t command,const std::chrono::seconds& duration)
{
	m_dataAvailable = false;
	std::unique_lock<std::mutex> lck(m_condVarMutex);
	m_condVar.wait_for(lck, duration, [&](){ return m_dataAvailable.load(); });
	std::lock_guard<std::mutex> guard(m_syncResponseMapMutex);
	auto mapIter = m_syncResponseMap.find(command);
	if(mapIter == m_syncResponseMap.end())
	{
		return std::unique_ptr<BaseObject>{};
	}
	auto retval =  std::move(mapIter->second);
	//Lets remove the entry from map and list
	m_syncResponseMap.erase(mapIter);
	removeRequestSyncResponse(command);
	return retval;
}
void Observer::removeRequestSyncResponse(uint16_t command)
{
	std::lock_guard<std::mutex> guard(m_syncRequestResponseListMutex);
	m_syncRequestResponseList.erase(std::remove(begin(m_syncRequestResponseList),
			end(m_syncRequestResponseList), command), end(m_syncRequestResponseList));
}


void Observer::subscribeAsynchCommand(uint16_t command, AsynchronousCallbackType callback)
{
	std::lock_guard<std::mutex> guard(m_registeredCommandsMapMutex);
	m_registeredCommandsMap[command] = callback;
}

void Observer::asynchronousThread()
{
	while(m_stopThread.load() == false)
	{
		std::unique_lock<std::mutex> lck(m_asynchronousCondVarMutex);
		m_asynchronousCondVar.wait(lck, [&](){ return m_asynchronousDataAvailable.load(); });
		//May be we have the data... Lets check.
		m_asynchronousDataAvailable = false;
		{
			std::lock_guard<std::mutex> guard(m_receivedCommandObjectsMutex);
			std::lock_guard<std::mutex> guard1(m_registeredCommandsMapMutex);
			for(auto& objectPtr : m_receivedCommandObjects)
			{
				auto command = objectPtr->getCommand();
				auto mapIter = m_registeredCommandsMap.find(command);
				if(mapIter == m_registeredCommandsMap.end())
				{
					//Nothing to do here... Continue
					continue;
				}
				//If here then we got a hit... Perform callback function.
				auto callback = m_registeredCommandsMap[command];
				callback(std::move(objectPtr));
			}
			//Once done then we clear the list
			m_receivedCommandObjects.clear();
		}
	}
}

Observer::~Observer()
{
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Destructor called \r\n");
	m_stopThread = true;
	//To get out of sleep
	m_asynchronousDataAvailable = true;
	m_asynchronousCondVar.notify_one();
	m_thread->join();
}

