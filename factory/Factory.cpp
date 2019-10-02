/*
 * Factory.cpp
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */
#include "../../simpleDebug/SimpleDebug.h"
#include "../../simpleSerial/utility/Utility.h"
#include "../object/sys/sync/SysVersionResponse.h"
#include "../object/sys/sync/SysOsalNvReadResponse.h"
#include "../object/mtUtil/sync/MtUtilGetDeviceInfoResponse.h"
#include "../object/general/sync/SyncGeneralResponse.h"
#include "../object/mtZdo/sync/MtZdoStartupFromAppResponse.h"
#include "../object/mtZdo/async/MtZdoAsyncNodeDescResponse.h"
#include "../object/mtZdo/async/MtZdoAsyncActiveEPResponse.h"
#include "../object/mtZdo/async/MtZdoAsyncEndDeviceAnnceInd.h"
#include "../object/mtZdo/async/MtZdoAsyncSimpleDescResponse.h"
#include "Factory.h"

using namespace SimpleZigbeeName;

Factory::Factory()
{
	m_debug = SimpleDebugName::SimpleDebug::instance();
}


std::unique_ptr<BaseObject> Factory::create(const std::vector<uint8_t>& data)
{
	//Lets see what type of message is there
	uint8_t command0 = data[CMD0_INDEX];
	uint8_t type = command0 & TYPE_MASK;
	uint8_t subSystem = command0 & SYB_SYSTEM_MASK;
	auto retVal = std::unique_ptr<BaseObject>{};
	switch(type)
	{
		case TYPE_SRSP:
		{
			switch(subSystem)
			{
				case SUBSYSTEM_SYS_INTERFACE:
				{
					SysCommandsEnum command1 = (SysCommandsEnum)data[CMD1_INDEX];
					retVal = std::move(createSysinterfaceResponse(command1,data));
					break;
				}
				case SUBSYSTEM_AF_INTERFACE:
				{
					MtAfCommandsEnum command1 = (MtAfCommandsEnum)data[CMD1_INDEX];
					retVal = std::move(createMtAfResponse(command1,data));
					break;
				}
				case SUBSYSTEM_ZDO_INTERFACE:
				{
					MtZdoCommandsEnum command1 = (MtZdoCommandsEnum)data[CMD1_INDEX];
					retVal = std::move(createMtZdoResponse(command1,data));
					break;
				}
				case SUBSYSTEM_UTIL_INTERFACE:
				{
					MtUtilCommandsEnum command1 = (MtUtilCommandsEnum)data[CMD1_INDEX];
					retVal = std::move(createMtUtilResponse(command1,data));
					break;
				}
			}
			break;
		}
		case TYPE_ARSP:
		{
			//for now just do one
			switch(subSystem)
			{
				case SUBSYSTEM_ZDO_INTERFACE:
				{
					MtZdoCommandsEnum command1 = (MtZdoCommandsEnum)data[CMD1_INDEX];
					retVal = std::move(createMtZdoResponse(command1,data));
					break;
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}
	if(!retVal)
	{
	//For now there is NO object that can be created
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : Nothing to handle below data \r\n");
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : " + SimpleSerialName::Utility::hexStr(data.data(),data.size()) + "\r\n");
	}
	return retVal;
}

std::unique_ptr<BaseObject> Factory::createSysinterfaceResponse(const SysCommandsEnum command, const std::vector<uint8_t>& data)
{
	auto retVal = std::unique_ptr<BaseObject>{};
	switch(command)
	{
		case SYS_VERSION:
		{
			auto retval1 = std::make_unique<SysVersionResponse>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}
		case SYS_OSAL_NV_READ:
		{
			auto retval1 = std::make_unique<SysOsalNvReadResponse>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}
		case SYS_PING:
		case SYS_SET_EXTADDR:
		case SYS_GET_EXTADDR:
		case SYS_RAM_READ:
		case SYS_RAM_WRITE:
		case SYS_OSAL_NV_ITEM_INIT:
		case SYS_OSAL_NV_WRITE:
		case SYS_OSAL_START_TIMER:
		case SYS_OSAL_STOP_TIMER:
		case SYS_RANDOM:
		case SYS_ADC_READ:
		case SYS_GPIO:
		case SYS_STACK_TUNE:
		case SYS_SET_TIME:
		case SYS_GET_TIME:
		case SYS_OSAL_NV_DELETE:
		case SYS_OSAL_NV_LENGTH:
		case SYS_SET_TX_POWER:
		case SYS_ZDIAGS_INIT_STATS:
		case SYS_ZDIAGS_CLEAR_STATS:
		case SYS_ZDIAGS_GET_STATS:
		case SYS_ZDIAGS_RESTORE_STATS_NV:
		case SYS_ZDIAGS_SAVE_STATS_TO_NV:
		case SYS_NV_CREATE:
		case SYS_NV_DELETE:
		case SYS_NV_LENGTH:
		case SYS_NV_READ:
		case SYS_NV_WRITE:
		case SYS_NV_UPDATE:
		case SYS_NV_COMPACT:
		default:
		{
			break;
		}
	}
	return retVal;
}


std::unique_ptr<BaseObject> Factory::createMtUtilResponse(const MtUtilCommandsEnum command, const std::vector<uint8_t>& data)
{
	auto retVal = std::unique_ptr<BaseObject>{};
	switch(command)
	{
		case MT_UTIL_GET_DEVICE_INFO:
		{
			auto retval1 = std::make_unique<MtUtilGetDeviceInfoResponse>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}
		case MT_UTIL_GET_NV_INFO:
		case MT_UTIL_SET_PANID:
		case MT_UTIL_SET_CHANNELS:
		case MT_UTIL_SET_SECLEVEL:
		case MT_UTIL_SET_PRECFGKEY:
		case MT_UTIL_CALLBACK_SUB_CMD:
		case MT_UTIL_KEY_EVENT:
		case MT_UTIL_TIME_ALIVE:
		case MT_UTIL_LED_CONTROL:
		case MT_UTIL_LOOPBACK:
		case MT_UTIL_DATA_REQ:
		case MT_UTIL_SRC_MATCH_ENABLE:
		case MT_UTIL_SRC_MATCH_ADD_ENTRY:
		case MT_UTIL_SRC_MATCH_DEL_ENTRY:
		case MT_UTIL_SRC_MATCH_CHECK_SRC_ADDR:
		case MT_UTIL_SRC_MATCH_ACK_ALL_PENDING:
		case MT_UTIL_SRC_MATCH_CHECK_ALL_PENDI:
		case MT_UTIL_ADDRMGR_EXT_ADDR_LOOKUP:
		case MT_UTIL_ADDRMGR_NWK_ADDR_LOOKUP:
		case MT_UTIL_APSME_LINK_KEY_DATA_GET:
		case MT_UTIL_APSME_LINK_KEY_NV_ID_GET:
		case MT_UTIL_APSME_REQUEST_KEY_CMD:
		{
			break;
		}
	}
	return retVal;
}

std::unique_ptr<BaseObject> Factory::createMtZdoResponse(const MtZdoCommandsEnum command, const std::vector<uint8_t>& data)
{
	auto retVal = std::unique_ptr<BaseObject>{};
	switch(command)
	{
		case ZDO_STARTUP_FROM_APP:
		{
			auto retval1 = std::make_unique<MtZdoStartupFromAppResponse>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}
		case ZDO_NWK_ADDR_REQ:
		case ZDO_IEEE_ADDR_REQ:
		case ZDO_NODE_DESC_REQ:
		case ZDO_POWER_DESC_REQ:
		case ZDO_SIMPLE_DESC_REQ:
		case ZDO_ACTIVE_EP_REQ:
		case ZDO_MATCH_DESC_REQ:
		case ZDO_COMPLEX_DESC_REQ:
		case ZDO_USER_DESC_REQ:
		case ZDO_END_DEVICE_ANNCE:
		case ZDO_USER_DESC_SET:
		case ZDO_SERVER_DISC_REQ:
		case ZDO_END_DEVICE_BIND_REQ:
		case ZDO_BIND_REQ:
		case ZDO_UNBIND_REQ:
		case ZDO_SET_LINK_KEY:
		case ZDO_REMOVE_LINK_KEY:		{
			auto retval1 = std::make_unique<SyncGeneralResponse>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}

		case ZDO_NODE_DESC_RSP:
		{
			auto retval1 = std::make_unique<MtZdoAsyncNodeDescResponse>();
			retval1->create(data);
			retVal = std::move(retval1);
			break;
		}

		case ZDO_ACTIVE_EP_RSP:
		{
			auto retval1 = std::make_unique<MtZdoAsyncActiveEPResponse>();
			retval1->create(data);
			retVal = std::move(retval1);
			break;
		}

		case ZDO_END_DEVICE_ANNCE_IND:
		{
			auto retval1 = std::make_unique<MtZdoAsyncEndDeviceAnnceInd>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}

		case ZDO_SIMPLE_DESC_RSP:
		{
			auto retval1 = std::make_unique<MtZdoAsyncSimpleDescResponse>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}

		case ZDO_GET_LINK_KEY:
		case ZDO_NWK_DISCOVERY_REQ_OR_ZDO_SET_REJOIN_PARAMETERS:
		case ZDO_JOIN_REQ:
		case ZDO_MGMT_NWK_DISC_REQ:
		case ZDO_MGMT_LQI_REQ:
		case ZDO_MGMT_RTG_REQ:
		case ZDO_MGMT_BIND_REQ:
		case ZDO_MGMT_LEAVE_REQ:
		case ZDO_MGMT_DIRECT_JOIN_REQ:
		case ZDO_MGMT_PERMIT_JOIN_REQ:
		case ZDO_MGMT_NWK_UPDATE_REQ:
		case ZDO_MSG_CB_REGISTER:
		case ZDO_MSG_CB_REMOVE:
		case ZDO_STARTUP_FROM_APP_EX:
		case ZDO_NWK_ADDR_RSP:
		case ZDO_IEEE_ADDR_RSP:
		case ZDO_POWER_DESC_RSP:
		case ZDO_MATCH_DESC_RSP:
		case ZDO_COMPLEX_DESC_RSP:
		case ZDO_USER_DESC_RSP:
		case ZDO_USER_DESC_CONF:
		case ZDO_SERVER_DISC_RSP:
		{
			break;
		}
	}
	return retVal;
}

std::unique_ptr<BaseObject> Factory::createMtAfResponse(const MtAfCommandsEnum command, const std::vector<uint8_t>& data)
{
	auto retVal = std::unique_ptr<BaseObject>{};
	switch(command)
	{
		case AF_REGISTER:
		{
			auto retval1 = std::make_unique<SyncGeneralResponse>();
			if(retval1->create(data) == true)
			{
				retVal = std::move(retval1);
			}
			break;
		}
		case AF_DATA_REQUEST:
		{
			break;
		}
	}
	return retVal;
}

