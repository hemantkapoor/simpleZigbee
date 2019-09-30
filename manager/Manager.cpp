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
#include "../object/sys/sync/SysOsalNvReadResponse.h"
#include "../object/mtZdo/async/MtZdoAsyncActiveEPResponse.h"
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
	auto respObject = m_observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1) );
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
	//auto getPreCnfgKey =  Utility::constructMessage(SYNC_MT_SAPI_COMMAND0, MT_SAPI_ZB_READ_CONFIGURATION, MessageDataType{0x62});
	//m_comms->transmitData(getPreCnfgKey);
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Starting Device as Coordinator\r\n");
	if(startCoordinator() == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot start Device as coordinator. Have you flashed the stick\r\n");
		return false;
	}

	//Not sure what we are writting but here goes
	auto dataTowrite = Utility::constructMessage(SYNC_SYS_COMMAND0, SYS_OSAL_NV_WRITE, MessageDataType{0x84, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00});
	m_comms->transmitData(dataTowrite);
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Device started as Coordinator\r\n");
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Getting Node Description\r\n");
	if(getNodeDescription(0,0) == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot Get Node description\r\n");
		return false;

	}
	auto activeEndPoint = getActiveEndPoints(0,0);
	//bool endPointResult;
	//std::vector<uint8_t> endpoints;
	auto [endPointResult,endpoints] = activeEndPoint;
	if(endPointResult == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot Get Active End Points\r\n");
		return false;
	}
	else
	{
		//Check if end points are present or now
		if(endpoints.empty())
		{
			m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : No end point found. Adding Now\r\n");
			//We will create end points
			AddEndPointStruct endPoint;
			endPoint.EndPoint = 1;
			endPoint.AppDeviceId = CONFIGURATION_TOOL;
			for(auto index=0;index<(uint16_t)(sizeof(PublicProfileIDs)/sizeof(uint16_t)); ++index)
			{
				endPoint.AppProfId = PublicProfileIDs[index];
				endPoint.EndPoint = index + 1;
				if(setActiveEndPoint(endPoint) == false)
				{
					m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Failed to set End Points\r\n");
					return false;
				}
			}
		}
	}

	//Lets get Node Description
	return true;
}

std::vector<uint8_t> ZigbeeManager::readOsalNvm(uint16_t id, uint8_t offset)
{

	std::vector<uint8_t> retData{};
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_SYS_COMMAND0,SYS_OSAL_NV_READ);
	m_observer->requestSyncResponse(responseCommandExpected);

	auto dataTosend =  Utility::constructMessage(SYNC_SYS_COMMAND0,SYS_OSAL_NV_READ,MessageDataType{(uint8_t)(id & 0xFF), (uint8_t)((id & 0xFF00) >> 8),offset});
	m_comms->transmitData(dataTosend);
	auto respObject = m_observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));
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
	auto respObject = m_observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));
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

bool ZigbeeManager::startCoordinator()
{
	int retryAttempt = 5;
	DeviceStateEnum deviceState(INIT_NOT_STARTED_AUTOMATICALLY);
	while(deviceState != STARTED_AS_ZIGBEE_COORDINATOR)
	{
		deviceState = getDeviceState();
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
			auto respObject = m_observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));
			if(!respObject)
			{
				//Remove and return
				m_observer->removeRequestSyncResponse(responseCommandExpected);
				if(--retryAttempt == 0)
				{
					m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : No Response for Start Device Command\r\n");
					return false;
				}
				m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : No Response for Start Device Command. Retrying\r\n");
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
				continue;
			}
			respObject->print();
		}
	}
	return true;
}

bool ZigbeeManager::getNodeDescription(uint16_t destinationAddress,uint16_t networkAddress)
{
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_NODE_DESC_REQ);
	m_observer->requestSyncResponse(responseCommandExpected);

	auto asyncResponseExpected = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_NODE_DESC_REQ);
	m_observer->requestSyncResponse(asyncResponseExpected);

	auto dataTosend =  Utility::constructMessage(SYNC_MT_ZDO_COMMAND0,ZDO_NODE_DESC_REQ,
			MessageDataType{(uint8_t)(destinationAddress & 0xFF), (uint8_t)((destinationAddress & 0xFF00) >> 8),
			(uint8_t)(networkAddress & 0xFF), (uint8_t)((networkAddress & 0xFF00) >> 8)
			});
	m_comms->transmitData(dataTosend);
	auto respObject = m_observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));
	if(!respObject)
	{
		//Remove and return
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No acknowledgement for Node Description\r\n");
		m_observer->removeRequestSyncResponse(responseCommandExpected);
		return false;
	}
	respObject->print();

	auto asyncrespObject = m_observer->getSyncResponse(asyncResponseExpected,std::chrono::seconds(1));
	if(!asyncrespObject)
	{
		//Remove and return
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No data received for Node Description\r\n");
		m_observer->removeRequestSyncResponse(asyncResponseExpected);
		return false;
	}
	asyncrespObject->print();
	return true;
}

std::pair<bool,std::vector<uint8_t>> ZigbeeManager::getActiveEndPoints(uint16_t destinationAddress,uint16_t networkAddress)
{
	std::vector<uint8_t> retVector{};
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_ACTIVE_EP_REQ);
	m_observer->requestSyncResponse(responseCommandExpected);

	auto asyncResponseExpected = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_ACTIVE_EP_REQ);
	m_observer->requestSyncResponse(asyncResponseExpected);

	auto dataTosend =  Utility::constructMessage(SYNC_MT_ZDO_COMMAND0,ZDO_ACTIVE_EP_REQ,
			MessageDataType{(uint8_t)(destinationAddress & 0xFF), (uint8_t)((destinationAddress & 0xFF00) >> 8),
			(uint8_t)(networkAddress & 0xFF), (uint8_t)((networkAddress & 0xFF00) >> 8)
			});
	m_comms->transmitData(dataTosend);
	auto respObject = m_observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));
	if(!respObject)
	{
		//Remove and return
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No acknowledgement for Active End Point\r\n");
		m_observer->removeRequestSyncResponse(responseCommandExpected);
		return std::make_pair(false,retVector);
	}
	respObject->print();

	auto asyncrespObject = m_observer->getSyncResponse(asyncResponseExpected,std::chrono::seconds(1));
	if(!asyncrespObject)
	{
		//Remove and return
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No data received for Active End Point\r\n");
		m_observer->removeRequestSyncResponse(asyncResponseExpected);
		return std::make_pair(false,retVector);
	}
	asyncrespObject->print();

	//Convert to get Active End points Object
	auto getActiveEndPoints = SimpleSerialName::Utility::dynamicConvert<BaseObject,MtZdoAsyncActiveEPResponse>(std::move(asyncrespObject));
	if(getActiveEndPoints)
	{
		retVector = getActiveEndPoints->getEndPoints();
		return std::make_pair(true,retVector);
	}
	else
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : Error converting to dynamic object\r\n");
	}

	return std::make_pair(false,retVector);
}

bool ZigbeeManager::setActiveEndPoint(const AddEndPointStruct& ep)
{
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_MT_AF_COMMAND0,AF_REGISTER);
	m_observer->requestSyncResponse(responseCommandExpected);
	//Time to populate the data
	MessageDataType data{ep.EndPoint};
	auto AppProfIdVec = Utility::getBigEndian(ep.AppProfId);
	std::copy(AppProfIdVec.begin(),AppProfIdVec.end(),std::back_inserter(data));
	auto AppDeviceIdVec = Utility::getBigEndian(ep.AppDeviceId);
	std::copy(AppDeviceIdVec.begin(),AppDeviceIdVec.end(),std::back_inserter(data));
	data.push_back(ep.AppDevVer);
	data.push_back(ep.LatencyReq);
	data.push_back(ep.AppNumInClusters);
	if(ep.AppNumInClusters >0 && ep.AppInClusterList != nullptr)
	{
		for(auto index=0;index<ep.AppNumInClusters;++index)
		{
			data.push_back(*(ep.AppInClusterList + index));
		}
	}
	data.push_back(ep.AppNumOutClusters);
	if(ep.AppNumOutClusters >0 && ep.AppOutClusterList != nullptr)
	{
		for(auto index=0;index<ep.AppNumOutClusters;++index)
		{
			data.push_back(*(ep.AppOutClusterList + index));
		}
	}
	auto dataTosend =  Utility::constructMessage(SYNC_MT_AF_COMMAND0,AF_REGISTER,data);
	m_comms->transmitData(dataTosend);
	auto respObject = m_observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));
	if(!respObject)
	{
		//Remove and return
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No acknowledgement for Adding End Point\r\n");
		m_observer->removeRequestSyncResponse(responseCommandExpected);
		return false;
	}
	respObject->print();
	return true;
}

ZigbeeManager::~ZigbeeManager()
{
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Destructor called \r\n");
	m_comms->removeCallback(m_zigbeeComms);
}

