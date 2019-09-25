/*
 * ZigbeeManager.cpp
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */
#include <vector>
#include "../../simpleDebug/SimpleDebug.h"
#include "../../simpleSerial/utility/Utility.h"
#include "../comms/Comms.h"
#include "../observer/Observer.h"
#include "../object/BaseObject.h"
#include "../object/SysOsalNvReadResponse.h"
#include "../utility/Utility.h"
#include "Manager.h"

using namespace SimpleZigbeeName;

ZigbeeManager::ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms):m_comms(comms)
{
	m_observer= std::make_shared<Observer>(comms);
	m_zigbeeComms = std::make_shared<ZigbeeComms>(comms,m_observer);
	m_comms->addCallback(m_zigbeeComms);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool ZigbeeManager::initialise()
{
	//First command is get the version
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Requesting Firmware Version\r\n");
	auto getVersion =  Utility::constructMessage(SYNC_SYS_COMMAND0, SYS_VERSION);
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_SYS_COMMAND0, SYS_VERSION);
	m_observer->requestSyncResponse(responseCommandExpected);
	m_comms->transmitData(getVersion);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(responseCommandExpected);
	if(!respObject)
	{
		//Remove and return
		m_observer->removeRequestSyncResponse(responseCommandExpected);
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : No Response for Firmware Version\r\n");
		return false;
	}
	respObject->print();

	//Next we Read NVM to check ZNP_HAS_CONFIGURED
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Checking if ZNP_HAS_CONFIGURED\r\n");
	auto nvmVal = readOsalNvm(ZNP_HAS_CONFIGURED_ADDRESS.id,ZNP_HAS_CONFIGURED_ADDRESS.offset);
	if(nvmVal.empty())
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot get ZNP_HAS_CONFIGURED value... Bailing\r\n");
		return false;
	}
	if(nvmVal[0] != 0x55)
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : ZNP has not been configured... Bailing\r\n");
		return false;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : ZNP has been configured...\r\n");


	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Reading PAN ID\r\n");
	nvmVal = readOsalNvm(ZCD_NV_PANID_ADDRESS.id,ZCD_NV_PANID_ADDRESS.offset);
	if(nvmVal.empty())
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot Read PAN ID... Bailing\r\n");

		return false;
	}

	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Reading Extended PAN ID\r\n");

	nvmVal = readOsalNvm(ZCD_NV_EXTENDED_PAN_ID_ADDRESS.id,ZCD_NV_EXTENDED_PAN_ID_ADDRESS.offset);
	if(nvmVal.empty())
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot Read Extended PAN ID... Bailing\r\n");
		return false;
	}

	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Reading Channel List\r\n");

	nvmVal = readOsalNvm(ZCD_NV_CHANLIST_ADDRESS.id,ZCD_NV_CHANLIST_ADDRESS.offset);
	if(nvmVal.empty())
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot Read Channel List... Bailing\r\n");
		return false;
	}

	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Reading ZCD_NV_PRECFGKEYS_ENABLE\r\n");

	nvmVal = readOsalNvm(ZCD_NV_PRECFGKEYS_ENABLE_ADDRESS.id,ZCD_NV_PRECFGKEYS_ENABLE_ADDRESS.offset);
	if(nvmVal.empty())
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot Read ZCD_NV_PRECFGKEYS_ENABLE... Bailing\r\n");
		return false;
	}

	//HK Not sure what this will do
	auto getPreCnfgKey =  Utility::constructMessage(SYNC_MT_SAPI_COMMAND0, MT_SAPI_ZB_READ_CONFIGURATION, MessageDataType{0x62});
	m_comms->transmitData(getPreCnfgKey);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Getting Device Ready\r\n");

	while(true)
	{
		auto deviceState = getDeviceState();
		if(deviceState == Device_ERROR)
		{
			m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot get Device State... Bailing\r\n");
			return false;
		}
		if(deviceState == INIT_NOT_STARTED_AUTOMATICALLY)
		{
			//We start the device
			m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Starting Device\r\n");
			auto transmitMessage =  Utility::constructMessage(SYNC_MT_ZDO_COMMAND0, ZDO_STARTUP_FROM_APP, MessageDataType{0x00, 0x03});
			auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_MT_ZDO_COMMAND0, ZDO_STARTUP_FROM_APP);
			m_observer->requestSyncResponse(responseCommandExpected);
			m_comms->transmitData(transmitMessage);
			//Lets sleep for a seconds
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			auto respObject = m_observer->getSyncResponse(responseCommandExpected);
			if(!respObject)
			{
				//Remove and return
				m_observer->removeRequestSyncResponse(responseCommandExpected);
				m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : No Response for Start Device Command\r\n");
				return false;
			}
			respObject->print();
		}
		else
		{
			//Break from while Loop
			break;
		}
	}

	return true;
}

std::vector<uint8_t> ZigbeeManager::readOsalNvm(uint16_t id, uint8_t offset)
{
	std::vector<uint8_t> retData{};
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_SYS_COMMAND0,SYS_OSAL_NV_READ);
	m_observer->requestSyncResponse(responseCommandExpected);

	auto dataTosend =  Utility::constructMessage(SYNC_SYS_COMMAND0,SYS_OSAL_NV_READ,MessageDataType{(uint8_t)(id & 0xFF), (uint8_t)((id & 0xFF00) >> 8),offset});
	m_comms->transmitData(dataTosend);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(responseCommandExpected);
	if(!respObject)
	{
		//Remove and return
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No response for Requesting NVM User Data\r\n");
		m_observer->removeRequestSyncResponse(responseCommandExpected);
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
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : Error converting to dynamic object\r\n");
	}
	return retData;
}

DeviceStateEnum ZigbeeManager::getDeviceState()
{
	DeviceStateEnum retState(Device_ERROR);

	auto retValue = getDeviceInfo();
	if(std::get<0>(retValue) == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : Cannot get Device Info... Bailing\r\n");
		return retState;
	}
	auto deviceInfoPair = std::get<1>(retValue);
	//Lets get the deviceInfo
	auto deviceInfo = std::get<0>(deviceInfoPair);
	//Now check system state
	retState = deviceInfo.DeviceState;

	return retState;
}

DevReturnType ZigbeeManager::getDeviceInfo()
{
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Requesting Device Information\r\n");
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_MT_UTIL_COMMAND0,MT_UTIL_GET_DEVICE_INFO);
	m_observer->requestSyncResponse(responseCommandExpected);

	auto devInfoRequestMessage =  Utility::constructMessage(SYNC_MT_UTIL_COMMAND0,MT_UTIL_GET_DEVICE_INFO);
	m_comms->transmitData(devInfoRequestMessage);
	//Lets sleep for a seconds
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto respObject = m_observer->getSyncResponse(responseCommandExpected);
	DeviceInfoStruct t1;
	std::vector<uint16_t> t2;
	auto retVal = std::make_pair(t1,t2);
	if(!respObject)
	{
		//Remove and return
		m_observer->removeRequestSyncResponse(responseCommandExpected);
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No Response for Firmware Version\r\n");
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
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : Error converting to dynamic object\r\n");
	}
	return std::make_pair(false,retVal);
}

ZigbeeManager::~ZigbeeManager()
{
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Destructor called \r\n");
	m_comms->removeCallback(m_zigbeeComms);
}

