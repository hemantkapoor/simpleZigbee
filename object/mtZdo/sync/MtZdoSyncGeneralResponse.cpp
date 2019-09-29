/*
 * MtZdoSyncGeneralResponse.cpp
 *
 *  Created on: 26 Sep 2019
 *      Author: hemant
 */
#include <sstream>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"
#include "MtZdoSyncGeneralResponse.h"

namespace SimpleZigbeeName
{

MtZdoSyncGeneralResponse::MtZdoSyncGeneralResponse()
{
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool MtZdoSyncGeneralResponse::create(const std::vector<uint8_t>& data)
{
	m_command = Utility::getSyncyResponseCommand(data[CMD0_INDEX], data[CMD1_INDEX]);
	m_status = (DeviceStatusEnum)data[DATA_INDEX];
	return true;
}

void MtZdoSyncGeneralResponse::print()
{
	std::stringstream outputSting;
	outputSting <<__PRETTY_FUNCTION__<< " Status = " << m_status << std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

};
