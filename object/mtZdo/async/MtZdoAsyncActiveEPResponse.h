/*
 * MtZdoAsyncNodeDescResponse.h
 *
 *  Created on: 28 Sep 2019
 *      Author: hemant
 */

#pragma once

#include <ostream>
#include <vector>
#include "../../BaseObject.h"
namespace SimpleDebugName
{
	class SimpleDebug;
};


namespace SimpleZigbeeName
{

struct __attribute__ ((packed))  ActiveEndpointRspStruct
{
	uint16_t SrcAddress;
	DeviceStatusEnum Status;
	uint16_t NwkAddress;
	uint8_t ActiveEPCount=20;
};

class MtZdoAsyncActiveEPResponse: public BaseObject {
public:
	MtZdoAsyncActiveEPResponse();
	virtual ~MtZdoAsyncActiveEPResponse();

	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }
	std::vector<uint8_t> getEndPoints() const { return m_endPointList; }

private:
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
	ActiveEndpointRspStruct m_data;
	std::vector<uint8_t> m_endPointList;
	bool m_dataPopulated = false;
};

} /* namespace SimpleZigbeeName */

