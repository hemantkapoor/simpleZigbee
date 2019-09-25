/*
 * SysOsalNvReadResponse.h
 *
 *  Created on: 22 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_SYSOSALNVREADRESPONSE_H_
#define SRC_SIMPLEZIGBEE_OBJECT_SYSOSALNVREADRESPONSE_H_

#include "BaseObject.h"

namespace SimpleDebugName
{
	class SimpleDebug;
};

namespace SimpleZigbeeName {

const uint8_t SYS_OSAL_NV_READ_STATUS_INDEX = DATA_INDEX;
const uint8_t SYS_OSAL_NV_READ_LEN_INDEX = SYS_OSAL_NV_READ_STATUS_INDEX + 1;
const uint8_t SYS_OSAL_NV_READ_VALUE_INDEX = SYS_OSAL_NV_READ_LEN_INDEX + 1;

class SysOsalNvReadResponse: public BaseObject {
public:
	SysOsalNvReadResponse();
	virtual ~SysOsalNvReadResponse();

	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }
	std::vector<uint8_t> data() const {return m_valueVector; }

private:
	uint16_t m_command;
	bool m_dataPopulated = false;
	std::vector<uint8_t> m_valueVector;
	SimpleDebugName::SimpleDebug* m_debug;
};

} /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_OBJECT_SYSOSALNVREADRESPONSE_H_ */
