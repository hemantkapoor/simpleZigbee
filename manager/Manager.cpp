/*
 * ZigbeeManager.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include "../../simpleSerial/utility/Utility.h"
#include "../comms/Comms.h"
#include "../observer/Observer.h"
#include "../object/BaseObject.h"
#include "../object/SysOsalNvReadResponse.h"
#include "Manager.h"

using namespace SimpleZigbeeName;

ZigbeeManager::ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
	m_observer= std::make_shared<Observer>(comms);
	m_zigbeeComms = std::make_shared<ZigbeeComms>(comms,m_observer);
	m_comms->addCallback(m_zigbeeComms);
}

bool ZigbeeManager::initialise()
{
	//First command is get the version
	std::cout<<__PRETTY_FUNCTION__<< " : Requesting Firmware Version\r\n";
	m_observer->requestSyncResponse(GET_VERSION_RESPONSE_CMD);
	m_comms->transmitData(GET_VERSION);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(GET_VERSION_RESPONSE_CMD);
	if(!respObject)
	{
		//Remove and return
		m_observer->removeRequestSyncResponse(GET_VERSION_RESPONSE_CMD);
		std::cout<<__PRETTY_FUNCTION__<< " : No Response for Firmware Version\r\n";
		return false;
	}
	respObject->print();

	//Next we Read NVM to check ZNP_HAS_CONFIGURED
	std::cout<<__PRETTY_FUNCTION__<< " : Checking if ZNP_HAS_CONFIGURED\r\n";
	auto nvmVal = readOsalNvm(ZNP_HAS_CONFIGURED_ADDRESS.id,ZNP_HAS_CONFIGURED_ADDRESS.offset);
	if(nvmVal.empty())
	{
		std::cout<<__PRETTY_FUNCTION__<< " : Cannot get ZNP_HAS_CONFIGURED value... Bailing\r\n";
		return false;
	}
	if(nvmVal[0] != 0x55)
	{
		std::cout<<__PRETTY_FUNCTION__<< " : ZNP has not been configured... Bailing\r\n";
		return false;
	}
	std::cout<<__PRETTY_FUNCTION__<< " : ZNP has been configured...\r\n";


	std::cout<<__PRETTY_FUNCTION__<< " : Reading PAN ID\r\n";

	nvmVal = readOsalNvm(ZCD_NV_PANID_ADDRESS.id,ZCD_NV_PANID_ADDRESS.offset);
	if(nvmVal.empty())
	{
		std::cout<<__PRETTY_FUNCTION__<< " : Cannot Read PAN ID... Bailing\r\n";
		return false;
	}

	std::cout<<__PRETTY_FUNCTION__<< " : Reading Extended PAN ID\r\n";

	nvmVal = readOsalNvm(ZCD_NV_EXTENDED_PAN_ID_ADDRESS.id,ZCD_NV_EXTENDED_PAN_ID_ADDRESS.offset);
	if(nvmVal.empty())
	{
		std::cout<<__PRETTY_FUNCTION__<< " : Cannot Read Extended PAN ID... Bailing\r\n";
		return false;
	}

	std::cout<<__PRETTY_FUNCTION__<< " : Reading Channel List\r\n";

	nvmVal = readOsalNvm(ZCD_NV_CHANLIST_ADDRESS.id,ZCD_NV_CHANLIST_ADDRESS.offset);
	if(nvmVal.empty())
	{
		std::cout<<__PRETTY_FUNCTION__<< " : Cannot Read Channel List... Bailing\r\n";
		return false;
	}

	std::cout<<__PRETTY_FUNCTION__<< " : Reading ZCD_NV_PRECFGKEYS_ENABLE\r\n";

	nvmVal = readOsalNvm(ZCD_NV_PRECFGKEYS_ENABLE_ADDRESS.id,ZCD_NV_PRECFGKEYS_ENABLE_ADDRESS.offset);
	if(nvmVal.empty())
	{
		std::cout<<__PRETTY_FUNCTION__<< " : Cannot Read ZCD_NV_PRECFGKEYS_ENABLE... Bailing\r\n";
		return false;
	}

	//HK Not sure what this will do
	m_comms->transmitData(GET_PRE_CFG_KEY);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	auto retValue = getDeviceInfo();
	return true;
}

std::vector<uint8_t> ZigbeeManager::readOsalNvm(uint16_t id, uint8_t offset)
{
	std::cout<<__PRETTY_FUNCTION__<< " : Id 0x" <<std::hex<<int(id)<< ", Offset 0x" <<std::hex<<int(offset)<<std::endl;
	std::vector<uint8_t> retData{};
	std::vector<uint8_t> dataTosend{ (uint8_t)0x03, (uint8_t)0x21, (uint8_t)0x08, (uint8_t)(id & 0xFF), (uint8_t)((id & 0xFF00) >> 8),offset};
	//The checksum
	uint8_t fcs(0);
	for (auto data : dataTosend)
	{
		fcs = fcs ^ data;
	}
	dataTosend.push_back(fcs);

	//Now we add SOF
	auto startIt = dataTosend.begin();
	dataTosend.insert(startIt,0xFE);
	m_observer->requestSyncResponse(READ_NVM_RESPONSE_CMD);
	m_comms->transmitData(dataTosend);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(READ_NVM_RESPONSE_CMD);
	if(!respObject)
	{
		//Remove and return
		std::cout<<__PRETTY_FUNCTION__<< " : No response for Requesting NVM User Data\r\n";
		m_observer->removeRequestSyncResponse(READ_NVM_RESPONSE_CMD);
		return retData;
	}
	respObject->print();
	//Convert to Read NVM Object
	auto readNvmObj = SimpleSerialName::Utility::dynamicConvert<BaseObject,SysOsalNvReadResponse>(std::move(respObject));
	if(readNvmObj)
	{
		retData = readNvmObj->data();
	}
	else
	{
		std::cout<<__PRETTY_FUNCTION__<< " : Error converting to dynamic object\r\n";
	}
	return retData;
}

DevReturnType ZigbeeManager::getDeviceInfo()
{
	std::cout<<__PRETTY_FUNCTION__<< " : Requesting Device Information\r\n";
	m_observer->requestSyncResponse(GET_DEVICE_INFO_RESPONSE_CMD);
	m_comms->transmitData(GET_DEVICE_INFO);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(GET_DEVICE_INFO_RESPONSE_CMD);
	DeviceInfoStruct t1;
	std::vector<uint16_t> t2;
	auto retVal = std::make_pair(t1,t2);
	if(!respObject)
	{
		//Remove and return
		m_observer->removeRequestSyncResponse(GET_DEVICE_INFO_RESPONSE_CMD);
		std::cout<<__PRETTY_FUNCTION__<< " : No Response for Firmware Version\r\n";
		return std::make_pair(false,retVal);
	}
	respObject->print();

	auto getDevInfoObj = SimpleSerialName::Utility::dynamicConvert<BaseObject,MtUtilGetDeviceInfoResponse>(std::move(respObject));
	if(getDevInfoObj)
	{
		return std::make_pair(true,getDevInfoObj->getResult());
	}
	else
	{
		std::cout<<__PRETTY_FUNCTION__<< " : Error converting to dynamic object\r\n";
	}
	return std::make_pair(false,retVal);
}

ZigbeeManager::~ZigbeeManager()
{
	std::cout<<__PRETTY_FUNCTION__<< " : Destructor called \r\n";
	m_comms->removeCallback(m_zigbeeComms);
}

