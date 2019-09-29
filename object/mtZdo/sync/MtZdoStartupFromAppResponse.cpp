/*
 * MtZdoStartupFromAppResponse.cpp
 *
 *  Created on: 24 Sep 2019
 *      Author: hemant
 */
#include <sstream>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"
#include "MtZdoStartupFromAppResponse.h"

namespace SimpleZigbeeName {

MtZdoStartupFromAppResponse::MtZdoStartupFromAppResponse()
{
	m_command = Utility::getSyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_STARTUP_FROM_APP);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool MtZdoStartupFromAppResponse::create(const std::vector<uint8_t>& data)
{
	//Sanity Test
	if(data[CMD1_INDEX] != ZDO_STARTUP_FROM_APP)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " Command didn't match\r\n");
		return false;
	}
	m_status = (ZdoStartupStatus)data[DATA_INDEX];
	return true;
}

void MtZdoStartupFromAppResponse::print()
{
	std::stringstream outputSting;
	outputSting <<__PRETTY_FUNCTION__<< " Status = " << m_status << std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

MtZdoStartupFromAppResponse::~MtZdoStartupFromAppResponse()
{
}

} /* namespace SimpleZigbeeName */
