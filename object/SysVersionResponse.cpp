/*
 * SysVersionResponse.cpp
 *
 *  Created on: 22 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include <cstring>
#include "../utility/Utility.h"
#include "SysVersionResponse.h"

namespace SimpleZigbeeName {

SysVersionResponse::SysVersionResponse()
{
	m_command = Utility::getSyncyResponseCommand(SYNC_SYS_COMMAND0,SYS_VERSION);
}


bool SysVersionResponse::create(const std::vector<uint8_t>& data)
{
	//Sanity Test
	if(data[CMD1_INDEX] != SYS_VERSION) { return false; }
	//Time to populate data
	auto srcDataPtr = data.data();
	std::memcpy((uint8_t*)&m_version, srcDataPtr+DATA_INDEX,sizeof(m_version));
	m_dataPopulated = true;
	return true;
}
void SysVersionResponse::print()
{
	if(m_dataPopulated == false)
	{
		std::cout<<__PRETTY_FUNCTION__<< "Incorrect data" <<std::endl;
		return;
	}
	std::cout<<__PRETTY_FUNCTION__<< " Data is \r\n";
	std::cout<<"Transport protocol revision : 0x" << std::hex << int(m_version.TransportRev) << std::endl;
	std::cout<<"Product Id : 0x" << std::hex << int(m_version.Product) << std::endl;
	std::cout<<"Software major release number : 0x" << std::hex << int(m_version.MajorRel) << std::endl;
	std::cout<<"Software minor release number : 0x"<< std::hex << int(m_version.MinorRel) << std::endl;
	std::cout<<"Software maintenance release number : 0x"<< std::hex << int(m_version.MaintRel) << std::endl;
}

SysVersionResponse::~SysVersionResponse()
{
}

} /* namespace SimpleZigbeeName */
