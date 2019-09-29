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
		//Check if we have are waiting for this message
		auto listIter = std::find(m_syncRequestResponseList.begin(), m_syncRequestResponseList.end(), command);
		if(listIter == m_syncRequestResponseList.end())
		{
			return;
		}
		else
		{
			m_dataAvailable = true;
			m_condVar.notify_one();
		}
	}
	std::lock_guard<std::mutex> guard(m_syncResponseMapMutex);
	m_syncResponseMap[command] = std::move(messageObject);
	std::stringstream outputSting;
	outputSting <<__PRETTY_FUNCTION__<< " : Added Command Id 0x"<< std::hex << int(command) << std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
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

Observer::~Observer()
{
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Destructor called \r\n");
}

