/*
 * MtAfAsyncIncomingMessage.h
 *
 *  Created on: 08 Oct 2019
 *      Author: hemant
 */

#pragma once

#include <ostream>
#include "../../BaseObject.h"
namespace SimpleDebugName
{
	class SimpleDebug;
};


namespace SimpleZigbeeName {

struct __attribute__ ((packed)) AfIncomingMsgStruct
{
uint16_t GroupId;
uint16_t ClusterId;
uint16_t SrcAddr;
uint8_t SrcEndpoint;
uint8_t DstEndpoint;
uint8_t WasBroadcast;
uint8_t LinkQuality;
uint8_t SecurityUse;
uint32_t TimeStamp;
uint8_t TransSeqNumber;
uint8_t Len;
};

class MtAfAsyncIncomingMessage : public BaseObject
{
public:
	MtAfAsyncIncomingMessage();
	virtual ~MtAfAsyncIncomingMessage();

	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }
	std::vector<uint8_t> getData() const { return m_data; }

private:
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
	AfIncomingMsgStruct m_properties;
	std::vector<uint8_t> m_data;
	bool m_dataPopulated = false;
};

} /* namespace SimpleZigbeeName */

