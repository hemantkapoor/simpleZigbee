/*
 * MtAfAsyncIncomingMessage.cpp
 *
 *  Created on: 08 Oct 2019
 *      Author: hemant
 */
#include "MtAfAsyncIncomingMessage.h"

#include <sstream>
#include <cstring>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"

namespace SimpleZigbeeName {

MtAfAsyncIncomingMessage::MtAfAsyncIncomingMessage()
{
	m_command = Utility::getAsyncyResponseCommand(SYNC_MT_AF_COMMAND0,AF_DATA_REQUEST);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool MtAfAsyncIncomingMessage::create(const std::vector<uint8_t>& data)
{
	auto srcDataPtr = data.data() + DATA_INDEX;
	std::memcpy((uint8_t*)&m_properties, srcDataPtr,sizeof(m_properties));
	srcDataPtr += sizeof(m_properties);
	m_data = std::vector<uint8_t>(srcDataPtr, srcDataPtr+m_properties.Len);
	m_dataPopulated = true;
	return true;
}

void MtAfAsyncIncomingMessage::print()
{
	if(m_dataPopulated == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " Data print later  \r\n");
}

MtAfAsyncIncomingMessage::~MtAfAsyncIncomingMessage()
{
}

} /* namespace SimpleZigbeeName */
