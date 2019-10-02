/*
 * MtZdoAsyncSimpleDescResponse.cpp
 *
 *  Created on: 2 Oct 2019
 *      Author: hemant
 */
#include <sstream>
#include <cstring>
#include "../../../../simpleDebug/SimpleDebug.h"
#include "../../../utility/Utility.h"
#include "MtZdoAsyncSimpleDescResponse.h"

namespace SimpleZigbeeName {

MtZdoAsyncSimpleDescResponse::MtZdoAsyncSimpleDescResponse()
{
	m_command = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_SIMPLE_DESC_RSP);
	m_debug = SimpleDebugName::SimpleDebug::instance();
}

MtZdoAsyncSimpleDescResponse::~MtZdoAsyncSimpleDescResponse()
{
}

bool MtZdoAsyncSimpleDescResponse::create(const std::vector<uint8_t>& data)
{
	auto srcDataPtr = data.data();
	std::memcpy((uint8_t*)&m_data.data, srcDataPtr+DATA_INDEX,sizeof(m_data.data));
	auto currentIndex = DATA_INDEX + sizeof(m_data.data);
	for(auto index=0;index<m_data.data.NumInClusters;++index)
	{
		//m_data.InClusterList.push_back(data[currentIndex]);
		//++currentIndex;
		uint16_t clusterId = data[currentIndex];
		++currentIndex;
		clusterId = clusterId | ((uint16_t)data[currentIndex]<<8);
		++currentIndex;
		m_data.InClusterList.push_back(clusterId);

	}
	m_data.NumOutClusters = data[currentIndex];
	++currentIndex;
	for(auto index=0;index<m_data.NumOutClusters;++index)
	{
		uint16_t clusterId = data[currentIndex];
		++currentIndex;
		clusterId = clusterId | ((uint16_t)data[currentIndex]<<8);
		++currentIndex;
		m_data.OutClusterList.push_back(clusterId);
	}
	m_dataPopulated = true;
	return true;
}

void MtZdoAsyncSimpleDescResponse::print()
{

	if(m_dataPopulated == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + "Incorrect data\r\n");
		return;
	}

	if(m_data.data.NumInClusters == 0)
	{
		m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " No Input Clusters present \r\n");
	}
	else
	{
		std::stringstream outputSting;
		outputSting << std::string(__PRETTY_FUNCTION__) << " List of Input Clusters for end point 0x" <<std::hex << (int)m_data.data.Endpoint << std::endl;
		m_debug->log(SimpleDebugName::LOG, outputSting);
		outputSting.str(std::string());

		for(auto cluster : m_data.InClusterList)
		{
			outputSting<< "0x" << std::hex << (int)cluster << ", ";
		}
		m_debug->log(SimpleDebugName::LOG, outputSting);
	}

	if(m_data.NumOutClusters == 0)
	{
		m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " No Output Clusters present \r\n");
	}
	else
	{

		std::stringstream outputSting1;
		outputSting1 << std::string(__PRETTY_FUNCTION__) << " List of Output Clusters for end point 0x" <<std::hex << (int)m_data.data.Endpoint << std::endl;
		m_debug->log(SimpleDebugName::LOG, outputSting1);
		outputSting1.str(std::string());

		for(auto cluster : m_data.OutClusterList)
		{
			outputSting1<< "0x" << std::hex << (int)cluster << ", ";
		}
		m_debug->log(SimpleDebugName::LOG, outputSting1);
	}
}

} /* namespace SimpleZigbeeName */
