/*
 * MtZdoAsyncSimpleDescResponse.h
 *
 *  Created on: 2 Oct 2019
 *      Author: hemant
 */

#pragma once

#include "../../BaseObject.h"
namespace SimpleDebugName
{
	class SimpleDebug;
};

namespace SimpleZigbeeName
{

struct __attribute__ ((packed))   SimpleDescpData1Struct
{
	uint16_t SrcAddr;
	DeviceStatusEnum Status;
	uint16_t NwkAddr;
	uint8_t Len;
	uint8_t Endpoint;
	uint16_t ProfileId;
	uint16_t DeviceId;
	uint8_t DeviceVersion;
	uint8_t NumInClusters;
};


struct SimpleDescpStruct
{
	SimpleDescpData1Struct data;
	std::vector<uint16_t> InClusterList;
	uint8_t NumOutClusters;
	std::vector<uint16_t> OutClusterList;
};


class MtZdoAsyncSimpleDescResponse: public BaseObject
{
public:
	MtZdoAsyncSimpleDescResponse();
	virtual ~MtZdoAsyncSimpleDescResponse();
	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }
	std::vector<uint16_t> getInputClusterList() const { return m_data.InClusterList; }
	std::vector<uint16_t> getOutputClusterList() const { return m_data.OutClusterList; }

private:
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
	SimpleDescpStruct m_data;
	bool m_dataPopulated = false;

};

} /* namespace SimpleZigbeeName */

