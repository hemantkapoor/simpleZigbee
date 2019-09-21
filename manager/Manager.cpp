/*
 * ZigbeeManager.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include "../comms/Comms.h"
#include "../observer/Observer.h"
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
	m_comms->transmitData(GET_VERSION);
	return true;
}

ZigbeeManager::~ZigbeeManager()
{
	std::cout<<__PRETTY_FUNCTION__<< " : Destructor called \r\n";
	m_comms->removeCallback(m_zigbeeComms);
}

