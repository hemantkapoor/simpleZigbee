/*
 * MtZdoAsyncNodeDescResponse.h
 *
 *  Created on: 28 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_MTZDOASYNCNODEDESCRESPONSE_H_
#define SRC_SIMPLEZIGBEE_OBJECT_MTZDOASYNCNODEDESCRESPONSE_H_

#include <ostream>
#include "BaseObject.h"
namespace SimpleDebugName
{
	class SimpleDebug;
};


namespace SimpleZigbeeName
{

enum LogicalTypeEnum : uint8_t
{
	ZigBeeCoordinator=0,
	ZigBeeRouter,
	ZigBeeEndDevice
};


struct __attribute__ ((packed))  NodeDescriptorFormat_t
{
	uint8_t LogicalType:3;
	uint8_t ComplexDescAvail:1;  /* AF_V1_SUPPORT - reserved bit. */
	uint8_t UserDescAvail:1;     /* AF_V1_SUPPORT - reserved bit. */
	uint8_t Reserved:3;
	uint8_t APSFlags:3;
	uint8_t FrequencyBand:5;
	uint8_t CapabilityFlags;
	uint16_t ManufacturerCode;
	uint8_t MaxBufferSize;
	uint16_t MaxInTransferSize;
	uint16_t ServerMask;
	uint16_t MaxOutTransferSize;
	uint8_t DescriptorCapability;
};

struct __attribute__ ((packed)) ZDO_NodeDescRsp_t
{
  uint8_t status;
  uint16_t nwkAddr;   // Network address of interest
  NodeDescriptorFormat_t nodeDesc;
};

class MtZdoAsyncNodeDescResponse: public BaseObject {
public:
	MtZdoAsyncNodeDescResponse();
	virtual ~MtZdoAsyncNodeDescResponse();

	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }

private:
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
	ZDO_NodeDescRsp_t m_data;
	bool m_dataPopulated = false;


	std::string getDeviceType(LogicalTypeEnum);
};

} /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_OBJECT_MTZDOASYNCNODEDESCRESPONSE_H_ */
