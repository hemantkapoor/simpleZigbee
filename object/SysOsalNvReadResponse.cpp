/*
 * SysOsalNvReadResponse.cpp
 *
 *  Created on: 22 Sep 2019
 *      Author: hemant
 */
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>
#include "../../simpleDebug/SimpleDebug.h"
#include "../utility/Utility.h"
#include "SysOsalNvReadResponse.h"

namespace SimpleZigbeeName {

SysOsalNvReadResponse::SysOsalNvReadResponse()
{
	m_command = Utility::getSyncyResponseCommand(SYNC_SYS_COMMAND0,SYS_OSAL_NV_READ);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool SysOsalNvReadResponse::create(const std::vector<uint8_t>& data)
{
	//Sanity Test
	if(data[CMD1_INDEX] != SYS_OSAL_NV_READ)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " Command didn't match\r\n");
		return false;
	}
	if(data[SYS_OSAL_NV_READ_STATUS_INDEX] == STATUS_FAIL)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " Staus failed\r\n");
		return false;
	}
	//Time to populate data
	auto dataLength = data[SYS_OSAL_NV_READ_LEN_INDEX];
	m_valueVector.clear();
	if(dataLength > 0)
	{
		auto startIter = data.begin() + SYS_OSAL_NV_READ_VALUE_INDEX;
		auto endIter = startIter + dataLength;
		std::copy(startIter, endIter, std::back_inserter(m_valueVector));
	}
	m_dataPopulated = true;
	return true;
}

void SysOsalNvReadResponse::print()
{
	if(m_valueVector.empty())
	{
		m_debug->log(SimpleDebugName::WARNING, std::string(__PRETTY_FUNCTION__) + " No data is present\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " Data is \r\n");
	std::stringstream outputSting;
	for(auto dataVal : m_valueVector)
	{
		outputSting << "0x" << std::hex << int(dataVal) << " ";
	}
	outputSting << std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

SysOsalNvReadResponse::~SysOsalNvReadResponse()
{
}

} /* namespace SimpleZigbeeName */
