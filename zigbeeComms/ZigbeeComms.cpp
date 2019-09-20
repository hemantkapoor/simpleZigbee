/*
 * ZigbeeComms.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include "../../simpleSerial/utility/Utility.h"
#include "ZigbeeComms.h"

using namespace SimpleZigbeeName;

ZigbeeComms::ZigbeeComms(std::shared_ptr<SimpleSerialName::Comms> comms, std::shared_ptr<Observer> observer):m_comms(comms),
		m_observer(observer)
{
}


ZigbeeComms::~ZigbeeComms()
{
	// TODO Auto-generated destructor stub
}

void ZigbeeComms::startParse()
{
	m_comms->addCallback(std::shared_ptr<ZigbeeComms>(this));
}

void ZigbeeComms::callback(std::vector<uint8_t>& data)
{
	//For test purpose
	std::cout<<__PRETTY_FUNCTION__<< " : " << SimpleSerialName::Utility::hexStr(data.data(),data.size())<<std::endl;
}


