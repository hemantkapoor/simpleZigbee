/*
 * Observer.cpp
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include <vector>
#include "../../simpleSerial/utility/Utility.h"
#include "Observer.h"

using namespace SimpleZigbeeName;

Observer::Observer(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
}

void Observer::handleReceivedMessage(std::unique_ptr<MessageDataType> message)
{
	//For now just print it
	std::cout<<__PRETTY_FUNCTION__<< " : " << SimpleSerialName::Utility::hexStr(message->data(),message->size());
}

Observer::~Observer()
{
	// TODO Auto-generated destructor stub

}

