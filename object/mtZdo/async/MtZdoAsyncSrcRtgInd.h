/*
 * MtZdoAsyncSrcRtgInd.h
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

class MtZdoAsyncSrcRtgInd : public BaseObject
{
public:
	MtZdoAsyncSrcRtgInd();
	virtual ~MtZdoAsyncSrcRtgInd();

	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }

private:
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
	uint16_t m_destAddress=0;
	uint8_t m_relayCount = 0;
	std::vector<uint16_t> m_relayList;
	bool m_dataPopulated = false;
};

} /* namespace SimpleZigbeeName */

