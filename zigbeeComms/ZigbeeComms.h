/*
 * ZigbeeComms.h
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_ZIGBEECOMMS_ZIGBEECOMMS_H_
#define SRC_SIMPLEZIGBEE_ZIGBEECOMMS_ZIGBEECOMMS_H_
#include "../../simpleSerial/comms/Comms.h"
#include "../../simpleSerial/callback/BaseCallback.h"
#include<memory>

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName
{
	class Observer;
	class ZigbeeComms : public SimpleSerialName::BaseCallback, std::enable_shared_from_this<ZigbeeComms>
	{
	public:
		ZigbeeComms() = delete;
		ZigbeeComms(std::shared_ptr<SimpleSerialName::Comms> comms, std::shared_ptr<Observer> observer);
		virtual ~ZigbeeComms();

		void startParse();

		void callback(std::vector<uint8_t>& data) override;
	    std::shared_ptr<ZigbeeComms> getptr() {
	        return shared_from_this();
	    }

	private:
		std::shared_ptr<SimpleSerialName::Comms> m_comms;
		std::shared_ptr<Observer>  m_observer;



	};

};

#endif /* SRC_SIMPLEZIGBEE_ZIGBEECOMMS_ZIGBEECOMMS_H_ */
