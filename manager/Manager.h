/*
 * ZigbeeManager.h
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_MANAGER_MANAGER_H_
#define SRC_SIMPLEZIGBEE_MANAGER_MANAGER_H_

#include <vector>
#include <memory>

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName

{

	class ZigbeeComms;
	class Observer;

	using MessageDataType = std::vector<uint8_t>;
	const MessageDataType GET_VERSION = {0xFE, 0x00, 0x21, 0x02, 0x23};
	const MessageDataType READ_NVM_USER_APP = {0xFE, 0x03, 0x21, 0x08, 0x00, 0x0F, 0x00, 0x25 };


	//Response Message Commands
	const uint16_t GET_VERSION_RESPONSE_CMD = 0x6102;
	const uint16_t READ_NVM_RESPONSE_CMD = 0x6108;


	class ZigbeeManager {
	public:
		ZigbeeManager() = delete;
		ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms);
		virtual ~ZigbeeManager();

		bool initialise();

	private:
		std::shared_ptr<SimpleSerialName::Comms> m_comms;
		std::shared_ptr<ZigbeeComms> m_zigbeeComms;
		std::shared_ptr<Observer> m_observer;
	};

};

#endif /* SRC_SIMPLEZIGBEE_MANAGER_MANAGER_H_ */
