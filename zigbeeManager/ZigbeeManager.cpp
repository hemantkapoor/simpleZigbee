/*
 * ZigbeeManager.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include "../zigbeeComms/ZigbeeComms.h"
#include "ZigbeeManager.h"

using namespace SimpleZigbeeName;

ZigbeeManager::ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms)
{
	m_zigbeeComms = std::make_unique<ZigbeeComms>(comms);
}

bool ZigbeeManager::initialise()
{
	return true;
}

ZigbeeManager::~ZigbeeManager()
{
}

