/*
 * MtUtilGetDeviceInfoResponse.cpp
 *
 *  Created on: 23 Sep 2019
 *      Author: hemant
 */
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "../../simpleSerial/utility/Utility.h"
#include "MtUtilGetDeviceInfoResponse.h"

namespace SimpleZigbeeName
{

bool MtUtilGetDeviceInfoResponse::create(const std::vector<uint8_t>& data)
{
	//Sanity Test
	if(data[CMD1_INDEX] != MT_UTIL_GET_DEVICE_INFO)
	{
		std::cout<<__PRETTY_FUNCTION__<< " Command didn't match\r\n";
		return false;
	}
	if(data.size() < sizeof(m_deviceInfo))
	{
		std::cout<<__PRETTY_FUNCTION__<< " Data Less than expected\r\n";
		return false;
	}
	//Looks like we have data... Copy it
	std::memcpy((uint8_t*)&m_deviceInfo,data.data() + DATA_INDEX,sizeof(m_deviceInfo));
	//Lets copy assosiated device
	if(m_deviceInfo.NumAssocDevices > 0)
	{
		//create sub vector and send
		auto firstIter = data.begin() + DATA_INDEX + sizeof(m_deviceInfo);
		auto endIter = firstIter +  (m_deviceInfo.NumAssocDevices * 2);
		std::vector<uint8_t> deviceVector(firstIter,endIter);
		//Now convert it
		m_assocDevicesList = SimpleSerialName::Utility::convertU8toU16(deviceVector);
	}
	m_dataPopulated = true;
	return true;
}

void MtUtilGetDeviceInfoResponse::print()
{
	if(m_dataPopulated == false)
	{
		std::cout<<__PRETTY_FUNCTION__<< "Incorrect data" <<std::endl;
		return;
	}
	if(m_deviceInfo.Status == STATUS_FAIL)
	{
		std::cout<<__PRETTY_FUNCTION__<< "Device Status failed..." <<std::endl;
		return;
	}
	//For now we jut print Device State
	std::string devState;
	switch(m_deviceInfo.DeviceState)
	{
		case INIT_NOT_STARTED_AUTOMATICALLY:
		{
			devState = "INIT_NOT_STARTED_AUTOMATICALLY";
			break;
		}
		case INITIALIZED_NOT_CONNECTED_TO_ANYTHING:
		{
			devState = "INITIALIZED_NOT_CONNECTED_TO_ANYTHING";
			break;
		}
		case DISCOVERING_PAN_TO_JOIN:
		{
			devState = "DISCOVERING_PAN_TO_JOIN";
			break;
		}
		case JOINING_A_PAN:
		{
			devState = "JOINING_A_PAN";
			break;
		}
		case REJOINING_A_PAN: //ONLY_FOR_END_DEVICES:
		{
			devState = "REJOINING_A_PAN";
			break;
		}
		case JOINED_BUT_NOT_YET_AUTHENTICATED_BY_TRUST_CENTER:
		{
			devState = "JOINED_BUT_NOT_YET_AUTHENTICATED_BY_TRUST_CENTER";
			break;
		}
		case STARTED_AS_DEVICE_AFTER_AUTHENTICATION:
		{
			devState = "STARTED_AS_DEVICE_AFTER_AUTHENTICATION";
			break;
		}
		case DEVICE_JOINED: //_AUTHENTICATED_AND_IS_A_ROUTER:
		{
			devState = "DEVICE_JOINED";
			break;
		}
		case STARTING_AS_ZIGBEE_COORDINATOR:
		{
			devState = "STARTING_AS_ZIGBEE_COORDINATOR";
			break;
		}
		case STARTED_AS_ZIGBEE_COORDINATOR:
		{
			devState = "STARTED_AS_ZIGBEE_COORDINATOR";
			break;
		}
		case DEVICE_HAS_LOST_INFORMATION_ABOUT_ITS_PARENT:
		{
			devState = "DEVICE_HAS_LOST_INFORMATION_ABOUT_ITS_PARENT";
			break;
		}
	}
	std::cout<<__PRETTY_FUNCTION__<< "Device Type : " << std::hex << (int)m_deviceInfo.DeviceType << std::endl;
	std::cout<<__PRETTY_FUNCTION__<< "Device State : " << devState << std::endl;
}

DeviceInfoResult MtUtilGetDeviceInfoResponse::getResult()
{
	return std::make_pair(m_deviceInfo,m_assocDevicesList);
}

};
