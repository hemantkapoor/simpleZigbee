/*
 * MtZdoSyncGeneralResponse.h
 *
 *  Created on: 26 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_MTZDOSYNCGENERALRESPONSE_H_
#define SRC_SIMPLEZIGBEE_OBJECT_MTZDOSYNCGENERALRESPONSE_H_

#include "BaseObject.h"

namespace SimpleDebugName
{
	class SimpleDebug;
};

namespace SimpleZigbeeName {

class MtZdoSyncGeneralResponse: public BaseObject
{
public:
	MtZdoSyncGeneralResponse();
	virtual ~MtZdoSyncGeneralResponse() = default;
	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }

	DeviceStatusEnum status() const { return m_status;  }
private:
	uint16_t m_command = 0;
	SimpleDebugName::SimpleDebug* m_debug = nullptr;
	DeviceStatusEnum m_status = STATUS_FAIL;
};

} /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_OBJECT_MTZDOSYNCGENERALRESPONSE_H_ */
