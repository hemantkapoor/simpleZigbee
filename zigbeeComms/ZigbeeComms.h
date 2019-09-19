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

	class ZigbeeComms : public SimpleSerialName::BaseCallback
	{
	public:
		ZigbeeComms() = delete;
		ZigbeeComms(std::shared_ptr<SimpleSerialName::Comms> comms);
		virtual ~ZigbeeComms();

		void startParse();

		void callback(std::vector<uint8_t>& data) override;

	private:
		std::shared_ptr<SimpleSerialName::Comms> m_comms;



	};

};

#endif /* SRC_SIMPLEZIGBEE_ZIGBEECOMMS_ZIGBEECOMMS_H_ */
