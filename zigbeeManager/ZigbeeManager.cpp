/*
 * ZigbeeManager.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include "../observer/Observer.h"
#include "../zigbeeComms/ZigbeeComms.h"
#include "ZigbeeManager.h"

using namespace SimpleZigbeeName;

ZigbeeManager::ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms)
{
	m_observer= std::make_shared<Observer>(comms);
	m_zigbeeComms = std::make_shared<ZigbeeComms>(comms,m_observer);

}

bool ZigbeeManager::initialise()
{
	m_zigbeeComms->startParse();
	return true;
}

ZigbeeManager::~ZigbeeManager()
{
}

