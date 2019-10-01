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
#include "../object/mtUtil/sync/MtUtilGetDeviceInfoResponse.h"

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleDebugName
{
	class SimpleDebug;
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

	//Important data for end points
	//https://www.rfwireless-world.com/Terminology/Zigbee-Profile-ID-list.html
	//Zigbee Public Profile IDs
	const uint16_t PublicProfileIDs[] =
	{
		0x0101, //INDUSTRIAL_PLANT_MONITORING
		0x0104, //HOME_AUTOMATION
		0x105, //COMMERCIAL_BUILDING_AUTOMATION
		0x107, //TELECOM_APPLICATIONS
		0x108, //PERSONAL_HOME_AND_HOSPITAL_CARE
		0x109 //ADVANCED_METERING_INITIATIVE
	};
	/*
	const uint16_t INDUSTRIAL_PLANT_MONITORING = 0x0101;
	const uint16_t HOME_AUTOMATION = 0x0104;
	const uint16_t COMMERCIAL_BUILDING_AUTOMATION = 0x105;
	const uint16_t TELECOM_APPLICATIONS = 0x107;
	const uint16_t PERSONAL_HOME_AND_HOSPITAL_CARE = 0x108;
	const uint16_t ADVANCED_METERING_INITIATIVE = 0x109;
	*/
	//Generic Device Ids
	const uint16_t ON_OFF_SWITCH = 0x0000;
	const uint16_t LEVEL_CONTROL_SWITCH = 0x0001;
	const uint16_t ON_OFF_OUTPUT = 0x0002;
	const uint16_t LEVEL_CONTROLLABLE_OUTPUT = 0x0003;
	const uint16_t SCENE_SELECTOR = 0x0004;
	const uint16_t CONFIGURATION_TOOL = 0x0005;
	const uint16_t REMOTE_CONTROL = 0x0006;
	const uint16_t COMBINED_INTERFACE = 0x0007;
	const uint16_t RANGE_EXTENDER = 0x0008;
	const uint16_t MAINS_POWER_OUTLET_ = 0x0009;

	struct __attribute__ ((packed)) AddEndPointStruct
	{
		uint8_t EndPoint = 0;
		uint16_t AppProfId = 0;
		uint16_t AppDeviceId = ON_OFF_SWITCH;
		uint8_t AppDevVer = 1;
		uint8_t LatencyReq = 0;
		uint8_t AppNumInClusters = 0;
		uint8_t* AppInClusterList = nullptr;
		uint8_t AppNumOutClusters = 0;
		uint8_t* AppOutClusterList = nullptr;
	};


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
		SimpleDebugName::SimpleDebug* m_debug;


		//Helper functions
		//Empty Vector means something when wrong
		std::vector<uint8_t> readOsalNvm(uint16_t id, uint8_t offset);
		DeviceStateEnum getDeviceState();
		DevReturnType getDeviceInfo();
		bool startCoordinator();
		bool setActiveEndPoint(const AddEndPointStruct& ep);

	};
};

#endif /* SRC_SIMPLEZIGBEE_MANAGER_MANAGER_H_ */
