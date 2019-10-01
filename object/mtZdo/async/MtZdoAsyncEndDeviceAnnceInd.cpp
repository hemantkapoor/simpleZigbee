/*
 * MtZdoAsyncEndDeviceAnnceInd.cpp
 *
 *  Created on: 1 Oct 2019
 *      Author: hemant
 */
#include <sstream>
#include <cstring>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"
#include "MtZdoAsyncEndDeviceAnnceInd.h"

namespace SimpleZigbeeName {

MtZdoAsyncEndDeviceAnnceInd::MtZdoAsyncEndDeviceAnnceInd()
{
	m_command = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_END_DEVICE_ANNCE_IND);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

bool MtZdoAsyncEndDeviceAnnceInd::create(const std::vector<uint8_t>& data)
{
	auto srcDataPtr = data.data();
	std::memcpy((uint8_t*)&m_data, srcDataPtr+DATA_INDEX,sizeof(m_data));
	m_dataPopulated = true;
	return true;
}

void MtZdoAsyncEndDeviceAnnceInd::print()
{
	if(m_dataPopulated == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " Data is \r\n");
	std::stringstream outputSting;
	outputSting<<"Source Address = 0x" << std::hex << (int)m_data.SrcAddr <<std::endl;
	outputSting<<"Network Address = 0x" << std::hex << (int)m_data.NwkAddr <<std::endl;
	outputSting<<"Device Type = 0x" << std::hex << (int)m_data.DeviceType <<std::endl;
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

MtZdoAsyncEndDeviceAnnceInd::~MtZdoAsyncEndDeviceAnnceInd()
{
}

} /* namespace SimpleZigbeeName */
