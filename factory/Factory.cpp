/*
 * Factory.cpp
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include "../../simpleSerial/utility/Utility.h"
#include "../object/SysVersionResponse.h"
#include "Factory.h"

using namespace SimpleZigbeeName;


std::unique_ptr<BaseObject> Factory::create(const std::vector<uint8_t>& data)
{
	//Lets see what type of message is there
	uint8_t command0 = data[CMD0_INDEX];
	SysCommandsEnum command1 = (SysCommandsEnum)data[CMD1_INDEX];
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
					retVal = std::move(createSysinterfaceResponse(command1,data));
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
		std::cout << __PRETTY_FUNCTION__<< " : Nothing to handle below data "<<std::endl;
		std::cout<<__PRETTY_FUNCTION__<< " : " << SimpleSerialName::Utility::hexStr(data.data(),data.size())<<std::endl;
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
		case SYS_PING:
		case SYS_SET_EXTADDR:
		case SYS_GET_EXTADDR:
		case SYS_RAM_READ:
		case SYS_RAM_WRITE:
		case SYS_OSAL_NV_ITEM_INIT:
		case SYS_OSAL_NV_READ:
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
