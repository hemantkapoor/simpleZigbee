/*
 * ZigbeeComms.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include "../../simpleDebug/SimpleDebug.h"
#include "../observer/Observer.h"
#include "../object/BaseObject.h"
#include "../factory/Factory.h"
#include "Comms.h"


using namespace SimpleZigbeeName;

ZigbeeComms::ZigbeeComms(std::shared_ptr<SimpleSerialName::Comms> comms, std::shared_ptr<Observer> observer):
		m_comms(comms),
		m_observer(observer)
{
}


ZigbeeComms::~ZigbeeComms()
{
	// TODO Auto-generated destructor stub
	SimpleDebugName::SimpleDebug::instance()->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Destructor called \r\n");
}


void ZigbeeComms::callback(std::vector<uint8_t>& data)
{
	//Lets go through every byte and decide what needs to be done
	for(auto value : data)
	{
		switch(m_commsState)
		{
			case ZC_WAIT_SOF:
			{
				if(value != ZC_SOF_VALUE) { break; }
				m_receivedMessage.clear();
				m_receivedMessage.push_back(value);
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
				m_receivedMessage.push_back(value);
				m_payLoadLenghtRemaining = value; //1 byte of checksum
				m_commsState = ZC_WAIT_CMD0;
				break;
			}
			case ZC_WAIT_CMD0:
			{
				m_receivedMessage.push_back(value);
				m_commsState = ZC_WAIT_CMD1;
				break;
			}
			case ZC_WAIT_CMD1:
			{
				m_receivedMessage.push_back(value);
				if(m_payLoadLenghtRemaining == 0)
				{
					//We handle the command here then set state to SOF
					Factory myFactory;
					auto messageObject = std::move(myFactory.create(m_receivedMessage));
					if(messageObject)
					{
						m_observer->handleReceivedMessage(std::move(messageObject));
					}
					m_commsState = ZC_WAIT_SOF;
					break;
				}
				m_commsState = ZC_WAIT_PAYLOAD;
				break;
			}
			case ZC_WAIT_PAYLOAD:
			{
				m_receivedMessage.push_back(value);
				--m_payLoadLenghtRemaining;
				if(m_payLoadLenghtRemaining == 0)
				{
					//We handle the command here then set state to SOF
					Factory myFactory;
					auto messageObject = myFactory.create(m_receivedMessage);
					m_observer->handleReceivedMessage(std::move(messageObject));
					m_commsState = ZC_WAIT_SOF;
					break;
				}
				break;
			}
		}
	}
}


