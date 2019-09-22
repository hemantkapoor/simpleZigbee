/*
 * BaseObject.h
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_BASEOBJECT_H_
#define SRC_SIMPLEZIGBEE_OBJECT_BASEOBJECT_H_
#include <vector>
#include "stdint.h"
namespace SimpleZigbeeName {

//System Sub Commands
enum SysCommandsEnum : uint8_t
{
	SYS_RESET_REQ = 0x00,
	SYS_PING,
	SYS_VERSION,
	SYS_SET_EXTADDR,
	SYS_GET_EXTADDR,
	SYS_RAM_READ,
	SYS_RAM_WRITE,
	SYS_OSAL_NV_ITEM_INIT,
	SYS_OSAL_NV_READ,
	SYS_OSAL_NV_WRITE = 0x09,
	SYS_OSAL_START_TIMER = 0x0A,
	SYS_OSAL_STOP_TIMER,
	SYS_RANDOM,
	SYS_ADC_READ,
	SYS_GPIO,
	SYS_STACK_TUNE = 0x0F,
	SYS_SET_TIME,
	SYS_GET_TIME,
	SYS_OSAL_NV_DELETE = 0x12,
	SYS_OSAL_NV_LENGTH,
	SYS_SET_TX_POWER = 0x14,
	SYS_ZDIAGS_INIT_STATS = 0x17,
	SYS_ZDIAGS_CLEAR_STATS,
	SYS_ZDIAGS_GET_STATS,
	SYS_ZDIAGS_RESTORE_STATS_NV = 0x1A,
	SYS_ZDIAGS_SAVE_STATS_TO_NV,
	SYS_NV_CREATE = 0X30,
	SYS_NV_DELETE,
	SYS_NV_LENGTH,
	SYS_NV_READ = 0X33,
	SYS_NV_WRITE= 0X34,
	SYS_NV_UPDATE = 0X35,
	SYS_NV_COMPACT
};

//Lets place important indexes first
const uint8_t CMD0_INDEX = 2;
const uint8_t CMD1_INDEX = 3;
const uint8_t DATA_INDEX = 4;

class BaseObject
{
public:
	BaseObject() = default;
	virtual ~BaseObject() = default;
	//BaseObject(std::vector<uint8_t>&);
	virtual uint16_t getCommand() = 0;
	virtual bool create(const std::vector<uint8_t>&) = 0;
	virtual void print() = 0;
};

}; /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_OBJECT_BASEOBJECT_H_ */
