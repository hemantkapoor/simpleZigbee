/*
 * Observer.cpp
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include "../../simpleSerial/utility/Utility.h"
#include "../object/BaseObject.h"
#include "Observer.h"

using namespace SimpleZigbeeName;

Observer::Observer(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
}

void Observer::handleReceivedMessage(std::unique_ptr<BaseObject> messageObject)
{
	if(!messageObject)
	{
		std::cout<<__PRETTY_FUNCTION__<< " : No Object present"<<std::endl;
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
	}
	std::lock_guard<std::mutex> guard(m_syncResponseMapMutex);
	m_syncResponseMap[command] = std::move(messageObject);
	std::cout<<__PRETTY_FUNCTION__<< " : Added Command Id 0x"<< std::hex << int(command) << std::endl;
}

void Observer::requestSyncResponse(uint16_t command)
{
	std::lock_guard<std::mutex> guard(m_syncRequestResponseListMutex);
	m_syncRequestResponseList.push_back(command);
}
std::unique_ptr<BaseObject> Observer::getSyncResponse(uint16_t command)
{
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
	std::cout<<__PRETTY_FUNCTION__<< " : Destructor called \r\n";
}

