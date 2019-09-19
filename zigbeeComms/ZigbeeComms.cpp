/*
 * ZigbeeComms.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */

#include "ZigbeeComms.h"

using namespace SimpleZigbeeName;

ZigbeeComms::ZigbeeComms(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
}


ZigbeeComms::~ZigbeeComms()
{
	// TODO Auto-generated destructor stub
}

void ZigbeeComms::startParse()
{
	//m_comms->addCallback(this);
}

void ZigbeeComms::callback(std::vector<uint8_t>& data)
{
}


