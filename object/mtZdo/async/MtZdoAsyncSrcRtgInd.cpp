/*
 * MtZdoAsyncSrcRtgInd.cpp
 *
 *  Created on: 08 Oct 2019
 *      Author: hemant
 */
#include <sstream>
#include <cstring>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"
#include "MtZdoAsyncSrcRtgInd.h"

namespace SimpleZigbeeName {

MtZdoAsyncSrcRtgInd::MtZdoAsyncSrcRtgInd()
{
	m_command = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_SRC_RTG_IND);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool MtZdoAsyncSrcRtgInd::create(const std::vector<uint8_t>& data)
{
	auto srcDataPtr = data.data() + DATA_INDEX;
	std::memcpy((uint8_t*)&m_destAddress, srcDataPtr,sizeof(m_destAddress));
	srcDataPtr += sizeof(m_destAddress);
	m_relayCount = *srcDataPtr;
	++srcDataPtr;
	auto dataVector = std::vector<uint8_t>(srcDataPtr, srcDataPtr+(m_relayCount*2));
	m_relayList = Utility::convertData(dataVector);
	m_dataPopulated = true;
	return true;
}

void MtZdoAsyncSrcRtgInd::print()
{
	if(m_dataPopulated == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " Data is \r\n");
	std::stringstream outputSting;
	outputSting<<"Destination Address = 0x" << std::hex << (int)m_destAddress <<std::endl;
	outputSting<<"Relay count = " << m_relayCount <<std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

MtZdoAsyncSrcRtgInd::~MtZdoAsyncSrcRtgInd()
{
}

} /* namespace SimpleZigbeeName */
