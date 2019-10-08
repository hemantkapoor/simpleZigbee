/*
 * ZclHelper.cpp
 *
 *  Created on: 3 Oct 2019
 *      Author: hemant
 */
#include <string>
#include <sstream>
#include <cstring>
#include "../../simpleDebug/SimpleDebug.h"
#include "ZclHelper.h"

namespace SimpleZigbeeName
{

ZclHelper::ZclHelper()
{
	m_debug = SimpleDebugName::SimpleDebug::instance();
}


uint32_t ZclHelper::getZclData(ClisterLibraryDataTypesEnum dataType,uint8_t *data, ZclDataType& retVal)
{
	uint32_t returnLength(0);
	switch(dataType)
	{
	//Lets only implement what we need now
		case ZCL_DATA8:
		case ZCL_ENUM8:

		{
			retVal = *data;
			returnLength = 1;
			break;
		}
		case ZCL_STRING:
		{
			//First data is length of the string
			uint8_t returnLength = *data;
			++data;
			std::string val((char*)data,returnLength);
			retVal = val;
			break;
		}
		case ZCL_NODATA:
		case ZCL_DATA16:
		case ZCL_DATA24:
		case ZCL_DATA32:
		case ZCL_DATA40:
		case ZCL_DATA48:
		case ZCL_DATA56:
		case ZCL_DATA64:
		case ZCL_DATABOOL:
		case ZCL_MAP8:
		case ZCL_MAP16:
		case ZCL_MAP24:
		case ZCL_MAP32:
		case ZCL_MAP40:
		case ZCL_MAP48:
		case ZCL_MAP56:
		case ZCL_MAP64:
		case ZCL_UINT8:
		case ZCL_UINT16:
		case ZCL_UINT24:
		case ZCL_UINT32:
		case ZCL_UINT40:
		case ZCL_UINT48:
		case ZCL_UINT56:
		case ZCL_UINT64:
		case ZCL_INT8:
		case ZCL_INT16:
		case ZCL_INT24:
		case ZCL_INT32:
		case ZCL_INT40:
		case ZCL_INT48:
		case ZCL_INT56:
		case ZCL_INT64:
		case ZCL_ENUM16:
		case ZCL_SEMI:
		case ZCL_SINGLE:
		case ZCL_DOUBLE:
		case ZCL_OCTSTR:
		case ZCL_OCTSTR16:
		case ZCL_STRING16:
		case ZCL_ARRAY:
		case ZCL_STRUCT:
		case ZCL_SET:
		case ZCL_BAG:
		case ZCL_TOD:
		case ZCL_DATE:
		case ZCL_UTC:
		case ZCL_CLUSTERID:
		case ZCL_ATTRIBID:
		case ZCL_BACOID:
		case ZCL_EUI64:
		case ZCL_KEY128:
		case ZCL_UNK:
		{
			std::stringstream outputSting;
			outputSting << std::string(__PRETTY_FUNCTION__) << " : No conversion for 0x" << std::hex << (int)dataType << " yet \r\n";
			m_debug->log(SimpleDebugName::CRITICAL_WARNING, outputSting);
			break;
		}
	}
	return returnLength;
}


} /* namespace SimpleZigbeeName */
