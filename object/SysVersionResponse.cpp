/*
 * SysVersionResponse.cpp
 *
 *  Created on: 22 Sep 2019
 *      Author: hemant
 */
#include <sstream>
#include <cstring>
#include "../../simpleDebug/SimpleDebug.h"
#include "../utility/Utility.h"
#include "SysVersionResponse.h"

namespace SimpleZigbeeName {

SysVersionResponse::SysVersionResponse()
{
	m_command = Utility::getSyncyResponseCommand(SYNC_SYS_COMMAND0,SYS_VERSION);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}


bool SysVersionResponse::create(const std::vector<uint8_t>& data)
{
	//Sanity Test
	if(data[CMD1_INDEX] != SYS_VERSION)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " Command didn't match\r\n");
		return false;
	}
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
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " Data is \r\n");
	std::stringstream outputSting;
	outputSting<<"Transport protocol revision : 0x" << std::hex << int(m_version.TransportRev) << std::endl;
	outputSting<<"Product Id : 0x" << std::hex << int(m_version.Product) << std::endl;
	outputSting<<"Software major release number : 0x" << std::hex << int(m_version.MajorRel) << std::endl;
	outputSting<<"Software minor release number : 0x"<< std::hex << int(m_version.MinorRel) << std::endl;
	outputSting<<"Software maintenance release number : 0x"<< std::hex << int(m_version.MaintRel) << std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

SysVersionResponse::~SysVersionResponse()
{
}

} /* namespace SimpleZigbeeName */
