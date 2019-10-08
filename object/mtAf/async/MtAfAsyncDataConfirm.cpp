/*
 * MtAfAsyncDataConfirm.cpp
 *
 *  Created on: 08 Oct 2019
 *      Author: hemant
 */
#include "MtAfAsyncDataConfirm.h"

#include <sstream>
#include <cstring>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"

namespace SimpleZigbeeName {

MtAfAsyncDataConfirm::MtAfAsyncDataConfirm()
{
	m_command = Utility::getAsyncyResponseCommand(SYNC_MT_AF_COMMAND0,AF_DATA_CONFIRM);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool MtAfAsyncDataConfirm::create(const std::vector<uint8_t>& data)
{
	auto srcDataPtr = data.data() + DATA_INDEX;
	std::memcpy((uint8_t*)&m_data, srcDataPtr,sizeof(m_data));
	return true;
}

void MtAfAsyncDataConfirm::print()
{
	if(m_dataPopulated == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " Data print later  \r\n");
}

MtAfAsyncDataConfirm::~MtAfAsyncDataConfirm()
{
}

} /* namespace SimpleZigbeeName */
