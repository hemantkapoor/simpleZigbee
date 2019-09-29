/*
 * MtZdoAsyncNodeDescResponse.cpp
 *
 *  Created on: 28 Sep 2019
 *      Author: hemant
 */
#include <sstream>
#include <cstring>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"
#include "MtZdoAsyncActiveEPResponse.h"

namespace SimpleZigbeeName {

MtZdoAsyncActiveEPResponse::MtZdoAsyncActiveEPResponse()
{
	m_command = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_ACTIVE_EP_RSP);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

MtZdoAsyncActiveEPResponse::~MtZdoAsyncActiveEPResponse()
{
}

bool MtZdoAsyncActiveEPResponse::create(const std::vector<uint8_t>& data)
{
	auto srcDataPtr = data.data();
	std::memcpy((uint8_t*)&m_data, srcDataPtr+DATA_INDEX,sizeof(m_data));
	auto currentIndex = DATA_INDEX + sizeof(m_data);
	for(auto index=0;index<m_data.ActiveEPCount;++index)
	{
		m_endPointList.push_back(data[currentIndex]);
		++currentIndex;
	}
	m_dataPopulated = true;
	return true;
}

void MtZdoAsyncActiveEPResponse::print()
{
	if(m_dataPopulated == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " List of end points are \r\n");
	std::stringstream outputSting;
	for(auto endPoint : m_endPointList)
	{
		outputSting<< endPoint << " ";
	}
	m_debug->log(SimpleDebugName::LOG, outputSting);
}

} /* namespace SimpleZigbeeName */
