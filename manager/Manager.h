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
#include <utility>
#include "../object/MtUtilGetDeviceInfoResponse.h"

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName

{

	class ZigbeeComms;
	class Observer;
	using DevReturnType = std::pair<bool,DeviceInfoResult>;

	using MessageDataType = std::vector<uint8_t>;

	const uint16_t READ_NVM_RESPONSE_CMD = 0x6108;

	//Command and Response Message
	const MessageDataType GET_VERSION = {0xFE, 0x00, 0x21, 0x02, 0x23};
	const uint16_t GET_VERSION_RESPONSE_CMD = 0x6102;

	const MessageDataType GET_DEVICE_INFO = {0xFE, 0x00, 0x27, 0x00, 0x27};
	const uint16_t GET_DEVICE_INFO_RESPONSE_CMD = 0x6700;

	const MessageDataType GET_PRE_CFG_KEY = {0xFE, 0x01, 0x26, 0x04, 0x62, 0x41};






	//Important OSAL NVM Address
	struct OsalNvmAddress
	{
		uint16_t id;
		uint8_t offset;
	};
	const OsalNvmAddress  ZNP_HAS_CONFIGURED_ADDRESS{0x0F00,00};
	const OsalNvmAddress  ZCD_NV_PANID_ADDRESS{0x0083,00};
	const OsalNvmAddress  ZCD_NV_EXTENDED_PAN_ID_ADDRESS{0x002D,00};
	const OsalNvmAddress  ZCD_NV_CHANLIST_ADDRESS{0x0084,00};
	const OsalNvmAddress  ZCD_NV_PRECFGKEYS_ENABLE_ADDRESS{0x0063,00};


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


		//Helper functions
		//Empty Vector means something when wrong
		std::vector<uint8_t> readOsalNvm(uint16_t id, uint8_t offset);

		DevReturnType getDeviceInfo();
	};
};

#endif /* SRC_SIMPLEZIGBEE_MANAGER_MANAGER_H_ */
