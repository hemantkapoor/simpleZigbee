/*
 * Factory.h
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_FACTORY_FACTORY_H_
#define SRC_SIMPLEZIGBEE_FACTORY_FACTORY_H_

#include "stdint.h"
#include <vector>
#include<memory>
#include "../object/BaseObject.h"

namespace SimpleDebugName
{
	class SimpleDebug;
};

namespace SimpleZigbeeName
{

class BaseObject;


const uint8_t TYPE_MASK = 0xE0;
const uint8_t SYB_SYSTEM_MASK = 0x1F;

const uint8_t TYPE_SRSP		= 0x60;
const uint8_t TYPE_ARSP		= 0x40;

enum SubSystemType : uint8_t
{
	SUBSYSTEM_RESERVED = 0x00,
	SUBSYSTEM_SYS_INTERFACE,	//0x01
	SUBSYSTEM_MAC_INTERFACE,	//0x02
	SUBSYSTEM_NWK_INTERFACE,	//0x03
	SUBSYSTEM_AF_INTERFACE,		//0x04
	SUBSYSTEM_ZDO_INTERFACE,	//0x05
	SUBSYSTEM_SAPI_INTERFACE,	//0x06
	SUBSYSTEM_UTIL_INTERFACE,	//0x07
	SUBSYSTEM_DEBUG_INTERFACE,	//0x08
	SUBSYSTEM_APP_INTERFACE,	//0x09
	SUBSYSTEM_APP_CONFIG = 0x0F,
	SUBSYSTEM_GREENPOWER = 0x15,
};


class Factory {
public:
	Factory();
	virtual ~Factory() = default;
	std::unique_ptr<BaseObject> create(const std::vector<uint8_t>& data);

private:
	SimpleDebugName::SimpleDebug* m_debug;

	std::unique_ptr<BaseObject> createSysinterfaceResponse(const SysCommandsEnum command, const std::vector<uint8_t>& data);
	std::unique_ptr<BaseObject> createMtUtilResponse(const MtUtilCommandsEnum command, const std::vector<uint8_t>& data);
	std::unique_ptr<BaseObject> createMtZdoResponse(const MtZdoCommandsEnum command, const std::vector<uint8_t>& data);
	std::unique_ptr<BaseObject> createMtAfResponse(const MtAfCommandsEnum command, const std::vector<uint8_t>& data);
};

}; /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_FACTORY_FACTORY_H_ */
