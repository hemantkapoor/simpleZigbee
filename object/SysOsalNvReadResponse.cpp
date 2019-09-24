/*
 * SysOsalNvReadResponse.cpp
 *
 *  Created on: 22 Sep 2019
 *      Author: hemant
 */
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "../utility/Utility.h"
#include "SysOsalNvReadResponse.h"

namespace SimpleZigbeeName {

SysOsalNvReadResponse::SysOsalNvReadResponse()
{
	m_command = Utility::getSyncyResponseCommand(SYNC_SYS_COMMAND0,SYS_OSAL_NV_READ);
}

bool SysOsalNvReadResponse::create(const std::vector<uint8_t>& data)
{
	//Sanity Test
	if(data[CMD1_INDEX] != SYS_OSAL_NV_READ)
	{
		std::cout<<__PRETTY_FUNCTION__<< " Command didn't match\r\n";
		return false;
	}
	if(data[SYS_OSAL_NV_READ_STATUS_INDEX] == STATUS_FAIL)
	{
		std::cout<<__PRETTY_FUNCTION__<< " Staus failed\r\n";
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
		std::cout<<__PRETTY_FUNCTION__<< " No data is present\r\n";
		return;
	}
	std::cout<<__PRETTY_FUNCTION__<< " Data is \r\n";
	for(auto dataVal : m_valueVector)
	{
		std::cout<<"0x" << std::hex << int(dataVal) << " ";
	}
	std::cout<<std::endl;
}

SysOsalNvReadResponse::~SysOsalNvReadResponse()
{
}

} /* namespace SimpleZigbeeName */
