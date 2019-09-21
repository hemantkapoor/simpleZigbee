/*
 * ZigbeeComms.h
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_COMMS_COMMS_H_
#define SRC_SIMPLEZIGBEE_COMMS_COMMS_H_
#include "../../simpleSerial/comms/Comms.h"
#include "../../simpleSerial/callback/BaseCallback.h"
#include "stdint.h"
#include <vector>
#include<memory>

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName
{
	class Observer;

	enum COMMS_STATE_ENUM
	{
		ZC_WAIT_SOF = 0,
		ZC_WAIT_LEN,
		ZC_WAIT_CMD0,
		ZC_WAIT_CMD1,
		ZC_WAIT_PAYLOAD
	};
	const uint8_t ZC_SOF_VALUE = 0xFE;
	const uint32_t MAX_PAYLOAD_LENGTH = 250;


	class ZigbeeComms : public SimpleSerialName::BaseCallback, std::enable_shared_from_this<ZigbeeComms>
	{
	public:
		ZigbeeComms() = delete;
		ZigbeeComms(std::shared_ptr<SimpleSerialName::Comms> comms, std::shared_ptr<Observer> observer);
		virtual ~ZigbeeComms();

		void callback(std::vector<uint8_t>& data) override;
	    std::shared_ptr<ZigbeeComms> getptr() {
	        return shared_from_this();
	    }

	private:
		std::shared_ptr<SimpleSerialName::Comms> m_comms;
		std::shared_ptr<Observer>  m_observer;
		COMMS_STATE_ENUM m_commsState = ZC_WAIT_SOF;
		std::vector<uint8_t> m_receivedMessage;
		uint8_t m_payLoadLenghtRemaining = 0;
	};

};

#endif /* SRC_SIMPLEZIGBEE_COMMS_COMMS_H_ */
