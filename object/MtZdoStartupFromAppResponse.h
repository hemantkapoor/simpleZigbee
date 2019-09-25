/*
 * MtZdoStartupFromAppResponse.h
 *
 *  Created on: 24 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_MTZDOSTARTUPFROMAPPRESPONSE_H_
#define SRC_SIMPLEZIGBEE_OBJECT_MTZDOSTARTUPFROMAPPRESPONSE_H_

#include "BaseObject.h"

namespace SimpleDebugName
{
	class SimpleDebug;
};

namespace SimpleZigbeeName {

enum ZdoStartupStatus : uint8_t
{
	RESTORED_NETWORK_STATE = 0x00,
	NEW_NETWORK_STATE,
	LEAVE_AND_NOT_STARTED
};

class MtZdoStartupFromAppResponse: public BaseObject
{
public:
	MtZdoStartupFromAppResponse();
	virtual ~MtZdoStartupFromAppResponse();


	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	ZdoStartupStatus status() const { return m_status;  }
	virtual uint16_t getCommand() final { return m_command; }

private:
	uint16_t m_command;
	ZdoStartupStatus m_status = RESTORED_NETWORK_STATE;
	SimpleDebugName::SimpleDebug* m_debug;
};

} /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_OBJECT_MTZDOSTARTUPFROMAPPRESPONSE_H_ */
