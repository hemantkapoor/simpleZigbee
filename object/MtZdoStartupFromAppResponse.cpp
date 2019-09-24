/*
 * MtZdoStartupFromAppResponse.cpp
 *
 *  Created on: 24 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include "../utility/Utility.h"
#include "MtZdoStartupFromAppResponse.h"

namespace SimpleZigbeeName {

MtZdoStartupFromAppResponse::MtZdoStartupFromAppResponse()
{
	m_command = Utility::getSyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_STARTUP_FROM_APP);
}

bool MtZdoStartupFromAppResponse::create(const std::vector<uint8_t>& data)
{
	//Sanity Test
	if(data[CMD1_INDEX] != ZDO_STARTUP_FROM_APP)
	{
		std::cout<<__PRETTY_FUNCTION__<< " Command didn't match\r\n";
		return false;
	}
	m_status = (ZdoStartupStatus)data[DATA_INDEX];
	return true;
}

void MtZdoStartupFromAppResponse::print()
{
	std::cout<<__PRETTY_FUNCTION__<< " Status = " << m_status << std::endl;
}

MtZdoStartupFromAppResponse::~MtZdoStartupFromAppResponse()
{
}

} /* namespace SimpleZigbeeName */
