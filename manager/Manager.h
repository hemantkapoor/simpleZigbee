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
		DeviceStateEnum getDeviceState();
		DevReturnType getDeviceInfo();

	};
};

#endif /* SRC_SIMPLEZIGBEE_MANAGER_MANAGER_H_ */
