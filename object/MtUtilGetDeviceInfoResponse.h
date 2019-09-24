/*
 * MtUtilGetDeviceInfoResponse.h
 *
 *  Created on: 23 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_MTUTILGETDEVICEINFORESPONSE_H_
#define SRC_SIMPLEZIGBEE_OBJECT_MTUTILGETDEVICEINFORESPONSE_H_
#include <utility>
#include "BaseObject.h"

namespace SimpleZigbeeName
{

enum DeviceStateEnum : uint8_t
{
	INIT_NOT_STARTED_AUTOMATICALLY= 0,
	INITIALIZED_NOT_CONNECTED_TO_ANYTHING,
	DISCOVERING_PAN_TO_JOIN,
	JOINING_A_PAN,
	REJOINING_A_PAN, //ONLY_FOR_END_DEVICES,
	JOINED_BUT_NOT_YET_AUTHENTICATED_BY_TRUST_CENTER,
	STARTED_AS_DEVICE_AFTER_AUTHENTICATION,
	DEVICE_JOINED, //_AUTHENTICATED_AND_IS_A_ROUTER,
	STARTING_AS_ZIGBEE_COORDINATOR,
	STARTED_AS_ZIGBEE_COORDINATOR,
	DEVICE_HAS_LOST_INFORMATION_ABOUT_ITS_PARENT,
	Device_ERROR
};

struct __attribute__ ((packed)) DeviceInfoStruct
{
	DeviceStatusEnum Status;
	uint8_t IEEEAddr[8];
	uint16_t ShortAddr;
	DeviceTypeEnum DeviceType;
	DeviceStateEnum DeviceState;
	uint8_t NumAssocDevices;
};

using DeviceInfoResult = std::pair<DeviceInfoStruct, std::vector<uint16_t>>;

class MtUtilGetDeviceInfoResponse: public SimpleZigbeeName::BaseObject
{
public:
	MtUtilGetDeviceInfoResponse();
	virtual ~MtUtilGetDeviceInfoResponse() = default;
	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }
	bool valid() const {return m_dataPopulated;}
	DeviceInfoResult getResult();

private:
	DeviceInfoStruct m_deviceInfo;
	std::vector<uint16_t> m_assocDevicesList;
	bool m_dataPopulated = false;
	uint16_t m_command;
};

};

#endif /* SRC_SIMPLEZIGBEE_OBJECT_MTUTILGETDEVICEINFORESPONSE_H_ */
