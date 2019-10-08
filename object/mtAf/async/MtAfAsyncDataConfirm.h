/*
 * MtAfAsyncDataConfirm.h
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

struct __attribute__ ((packed)) AfDataConfirmStruct
{

DeviceStatusEnum status;
uint8_t Endpoint;
uint8_t TransId;
};

class MtAfAsyncDataConfirm : public BaseObject
{
public:
	MtAfAsyncDataConfirm();
	virtual ~MtAfAsyncDataConfirm();

	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }

private:
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
	AfDataConfirmStruct m_data;
	bool m_dataPopulated = false;
};

} /* namespace SimpleZigbeeName */
