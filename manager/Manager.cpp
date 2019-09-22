/*
 * ZigbeeManager.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include "../comms/Comms.h"
#include "../observer/Observer.h"
#include "../object/BaseObject.h"
#include "Manager.h"

using namespace SimpleZigbeeName;

ZigbeeManager::ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
	m_observer= std::make_shared<Observer>(comms);
	m_zigbeeComms = std::make_shared<ZigbeeComms>(comms,m_observer);
	m_comms->addCallback(m_zigbeeComms);
}

bool ZigbeeManager::initialise()
{
	//First command is get the version
	std::cout<<__PRETTY_FUNCTION__<< " : Requesting Firmware Version\r\n";
	m_observer->requestSyncResponse(GET_VERSION_RESPONSE_CMD);
	m_comms->transmitData(GET_VERSION);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(GET_VERSION_RESPONSE_CMD);
	if(!respObject)
	{
		//Remove and return
		m_observer->removeRequestSyncResponse(GET_VERSION_RESPONSE_CMD);
		std::cout<<__PRETTY_FUNCTION__<< " : No Response for Firmware Version\r\n";
		return false;
	}
	respObject->print();

	//Next we Read NVM and verify we get correct thingies
	std::cout<<__PRETTY_FUNCTION__<< " : Requesting NVM User Data\r\n";
	m_observer->requestSyncResponse(READ_NVM_RESPONSE_CMD);
	m_comms->transmitData(READ_NVM_USER_APP);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	respObject = m_observer->getSyncResponse(READ_NVM_RESPONSE_CMD);
	if(!respObject)
	{
		//Remove and return
		std::cout<<__PRETTY_FUNCTION__<< " : No response for Requesting NVM User Data\r\n";
		m_observer->removeRequestSyncResponse(READ_NVM_RESPONSE_CMD);
		return false;
	}
	respObject->print();


	return true;
}

ZigbeeManager::~ZigbeeManager()
{
	std::cout<<__PRETTY_FUNCTION__<< " : Destructor called \r\n";
	m_comms->removeCallback(m_zigbeeComms);
}

