/*
 * MtZdoAsyncNodeDescResponse.cpp
 *
 *  Created on: 28 Sep 2019
 *      Author: hemant
 */
#include <sstream>
#include <cstring>
#include "../../simpleDebug/SimpleDebug.h"
#include "../utility/Utility.h"
#include "MtZdoAsyncNodeDescResponse.h"

namespace SimpleZigbeeName {

MtZdoAsyncNodeDescResponse::MtZdoAsyncNodeDescResponse()
{
	m_command = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_NODE_DESC_RSP);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

MtZdoAsyncNodeDescResponse::~MtZdoAsyncNodeDescResponse()
{
}

bool MtZdoAsyncNodeDescResponse::create(const std::vector<uint8_t>& data)
{
	auto srcDataPtr = data.data();
	std::memcpy((uint8_t*)&m_data, srcDataPtr+DATA_INDEX,sizeof(m_data));
	m_dataPopulated = true;
	return true;
}

void MtZdoAsyncNodeDescResponse::print()
{
	if(m_dataPopulated == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " Data is \r\n");
	std::stringstream outputSting;
	outputSting<<"Logical Type = " << getDeviceType(LogicalTypeEnum(m_data.nodeDesc.LogicalType)) << std::endl;
	outputSting<<"Manufactured code = 0x"<< std::hex << m_data.nodeDesc.ManufacturerCode  << std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

std::string MtZdoAsyncNodeDescResponse::getDeviceType(LogicalTypeEnum val)
{
	std::string retStr;
	switch(val)
	{
		case ZigBeeCoordinator:
		{
			retStr = "ZigBeeCoordinator";
			break;
		}
		case ZigBeeRouter:
		{
			retStr = "ZigBeeRouter";
			break;
		}
		case ZigBeeEndDevice:
		{
			retStr = "ZigBeeEndDevice";
			break;
		}
	}
	return retStr;
}

} /* namespace SimpleZigbeeName */
