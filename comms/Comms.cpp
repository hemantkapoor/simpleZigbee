/*
 * ZigbeeComms.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include "Comms.h"

#include <iostream>
#include "../../simpleSerial/utility/Utility.h"

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
	//Lets go through every byte and decide what needs to be done
	for(auto value : data)
	{
		switch(m_commsState)
		{
			case ZC_WAIT_SOF:
			{
				if(value != ZC_SOF_VALUE) { break; }
				m_currentIndex = 0;
				m_receivedMessage[m_currentIndex++] = value;
				m_commsState = ZC_WAIT_LEN;
				break;
			}
			case ZC_WAIT_LEN:
			{
				if(value > MAX_PAYLOAD_LENGTH)
				{
					//Bail out
					m_commsState = ZC_WAIT_SOF;
					break;
				}
				m_receivedMessage[m_currentIndex++] = value;
				m_payLoadLenghtRemaining = value;
				m_commsState = ZC_WAIT_CMD0;
				break;
			}
			case ZC_WAIT_CMD0:
			{
				m_receivedMessage[m_currentIndex++] = value;
				m_commsState = ZC_WAIT_CMD1;
				break;
			}
			case ZC_WAIT_CMD1:
			{
				m_receivedMessage[m_currentIndex++] = value;
				if(m_payLoadLenghtRemaining == 0)
				{
					//We handle the command here then set state to SOF
					m_commsState = ZC_WAIT_SOF;
					break;
				}
				m_commsState = ZC_WAIT_PAYLOAD;
				break;
			}
			case ZC_WAIT_PAYLOAD:
			{
				m_receivedMessage[m_currentIndex++] = value;
				if(m_payLoadLenghtRemaining == 0)
				{
					//We handle the command here then set state to SOF
					m_commsState = ZC_WAIT_SOF;
					break;
				}
				break;
			}
		}
	}
}


