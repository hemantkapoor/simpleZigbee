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
	uint16_t respCommand = 0x6102;
	m_observer->requestSyncResponse(respCommand);
	m_comms->transmitData(GET_VERSION);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(respCommand);
	if(!respObject)
	{
		//Remove and return
		m_observer->removeRequestSyncResponse(respCommand);
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

