/*
 * MtZdoAsyncEndDeviceAnnceInd.h
 *
 *  Created on: 1 Oct 2019
 *      Author: hemant
 */

#pragma once

#include <ostream>
#include "../../BaseObject.h"
namespace SimpleDebugName
{
	class SimpleDebug;
};

struct __attribute__ ((packed))  EndDeviceAnnStruct
{
	uint16_t SrcAddr;
	uint16_t NwkAddr;
	uint8_t IEEEAddr[8];
	uint8_t AlternatePANCoordinator:1;
	uint8_t DeviceType:1;
	uint8_t PowerSourc:1;
	uint8_t ReceiverOnWhenIdle:1;
	uint8_t Reserved1:1;
	uint8_t Reserved2:1;
	uint8_t SecurityCapability:1;
	uint8_t Reserved3e:1;
};

namespace SimpleZigbeeName {

class MtZdoAsyncEndDeviceAnnceInd : public BaseObject
{
public:
	MtZdoAsyncEndDeviceAnnceInd();
	virtual ~MtZdoAsyncEndDeviceAnnceInd();

	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }

private:
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
	EndDeviceAnnStruct m_data;
	bool m_dataPopulated = false;
};

} /* namespace SimpleZigbeeName */

