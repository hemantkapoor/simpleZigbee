/*
 * ZclHelper.h
 *
 *  Created on: 3 Oct 2019
 *      Author: hemant
 *
 *      Reference ZigBee Cluster Library Specification - ZigBee Document – 075123
 *
 ********************************************************************************************************************************
*                         *                                   *                                       *                                    *                           *
* Bits: 8              *                0/16            *                  8                   *                8                  *        Variable      *
*                         *                                   *                                       *                                    *                           *
********************************************************************************************************************************
*                         *                                   *                                       *                                    *                           *
* Frame control  * Manufacturer code    *  Transaction sequence *   Command identifier  * Frame payload  *
*                         *                                   *            Number              *                                    *                           *
*                         *                                   *                                       *                                    *                           *
********************************************************************************************************************************
*                         *                                   *                                       *                                    *                           *
*                                                            ZCL header                                                        *    ZCL payload       *
*                         *                                   *                                       *                                    *                           *
********************************************************************************************************************************

 */

#pragma once

#include <stdint.h>
#include <variant>
#include <vector>
#include <string>

namespace SimpleDebugName
{
	class SimpleDebug;
};


namespace SimpleZigbeeName
{

using ZclDataType = std::variant<uint8_t, uint16_t, uint32_t, uint64_t, float, double, std::vector<uint8_t>, std::string>;

const uint16_t CLUSTER_REVISION_ATTR_ID = 0xFFFD;
const uint16_t ATTRIBUTE_REPORTING_STATUS = 0xFFFE;

enum AttributeReportingStatus : uint8_t
{
	Pending = 0x00,
	AttributeReportingComplete
};

enum ZCLCommandFramesIdEnum : uint8_t
{
	ReadAttributes = 0x00,
	ReadAttributesResponse,
	WriteAttributes,
	WriteAttributesUndivided,
	WriteAttributesResponse,
	WriteAttributesNoResponse,
	ConfigureReporting,
	ConfigureReportingResponse,
	ReadReportingConfiguration,
	ReadReportingConfigurationResponse,
	Reportattributes,
	DefaultResponse,
	DiscoverAttributes,
	DiscoverAttributesResponse,
	ReadAttributesStructured,
	WriteAttributesStructured,
	WriteAttributesStructuredresponse,
	DiscoverCommandsReceived,
	DiscoverCommandsReceivedResponse,
	DiscoverCommandsGenerated,
	DiscoverCommandsGeneratedResponse,
	DiscoverAttributesExtended,
	DiscoverAttributesExtendedResponse
};

enum ClisterLibraryDataTypesEnum
{
	ZCL_NODATA = 0x00,
	ZCL_DATA8 = 0x08,
	ZCL_DATA16 = 0x09,
	ZCL_DATA24 = 0x0a,
	ZCL_DATA32 = 0x0b,
	ZCL_DATA40 = 0x0c,
	ZCL_DATA48 = 0x0d,
	ZCL_DATA56 = 0x0e,
	ZCL_DATA64 = 0x0f,
	ZCL_DATABOOL = 0x10,
	ZCL_MAP8 = 0x18,
	ZCL_MAP16 = 0x19,
	ZCL_MAP24 = 0x1a,
	ZCL_MAP32 = 0x1b,
	ZCL_MAP40 = 0x1c,
	ZCL_MAP48 = 0x1d,
	ZCL_MAP56 = 0x1e,
	ZCL_MAP64 = 0x1f,
	ZCL_UINT8 = 0x20,
	ZCL_UINT16 = 0x21,
	ZCL_UINT24 = 0x22,
	ZCL_UINT32 = 0x23,
	ZCL_UINT40 = 0x24,
	ZCL_UINT48 = 0x25,
	ZCL_UINT56 = 0x26,
	ZCL_UINT64 = 0x27,
	ZCL_INT8 = 0x28,
	ZCL_INT16 = 0x29,
	ZCL_INT24 = 0x2a,
	ZCL_INT32 = 0x2b,
	ZCL_INT40 = 0x2c,
	ZCL_INT48 = 0x2d,
	ZCL_INT56 = 0x2e,
	ZCL_INT64 = 0x2f,
	ZCL_ENUM8 = 0x30,
	ZCL_ENUM16 = 0x31,
	ZCL_SEMI = 0x38,
	ZCL_SINGLE = 0x39,
	ZCL_DOUBLE = 0x3a,
	ZCL_OCTSTR = 0x41,
	ZCL_STRING = 0x42,
	ZCL_OCTSTR16 = 0x43,
	ZCL_STRING16 = 0x44,
	ZCL_ARRAY = 0x48,
	ZCL_STRUCT = 0x4c,
	ZCL_SET = 0x50,
	ZCL_BAG = 0x51,
	ZCL_TOD = 0xe0,
	ZCL_DATE = 0xe1,
	ZCL_UTC = 0xe2,
	ZCL_CLUSTERID = 0xe8,
	ZCL_ATTRIBID = 0xe9,
	ZCL_BACOID = 0xea,
	ZCL_EUI64 = 0xf0,
	ZCL_KEY128 = 0xf1,
	ZCL_UNK = 0xff
};

struct __attribute__ ((packed)) ZclFrameControlFieldStruct
{
	uint8_t FrameType : 2;
	uint8_t ManufacturerSpecific : 1;
	uint8_t Direction : 1;
	uint8_t DisableDefaultResponse  : 1;
	uint8_t Reserved : 3;
};

struct __attribute__ ((packed)) ZclHeaderStruct
{
	ZclFrameControlFieldStruct FrameControl;
	//We will leave Manufacturer code as of now
	uint8_t TransactionSequence;
	uint8_t CommandIdentifier;
};

class ZclHelper
{
public:
	ZclHelper();
	virtual ~ZclHelper() = default;
	ZclDataType getZclData(ClisterLibraryDataTypesEnum,uint8_t *data);
private:
	SimpleDebugName::SimpleDebug* m_debug;
};

} /* namespace SimpleZigbeeName */

