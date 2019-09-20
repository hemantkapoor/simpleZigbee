/*
 * ZigbeeManager.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include "Manager.h"

#include "../comms/Comms.h"
#include "../observer/Observer.h"

using namespace SimpleZigbeeName;

ZigbeeManager::ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
	m_observer= std::make_shared<Observer>(comms);
	m_zigbeeComms = std::make_shared<ZigbeeComms>(comms,m_observer);

}

bool ZigbeeManager::initialise()
{
	m_zigbeeComms->startParse();
	//First command is get the version
	m_comms->transmitData(GET_VERSION);
	return true;
}

ZigbeeManager::~ZigbeeManager()
{
}

