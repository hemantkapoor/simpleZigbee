/*
 * ZclReadAttributeResponse.cpp
 *
 *  Created on: 8 Oct 2019
 *      Author: hemant
 */
#include <cstring>
#include "../../simpleDebug/SimpleDebug.h"
#include "../utility/Utility.h"
#include "../object/BaseObject.h"
#include "ZclHelper.h"
#include "ZclReadAttributeResponse.h"

namespace SimpleZigbeeName
{

ZclReadAttributeResponse::ZclReadAttributeResponse(const std::vector<uint8_t>& data)
{
	auto debug = SimpleDebugName::SimpleDebug::instance();
	//Some sanity test here
	if(data.size() < sizeof(ZclHeaderStruct))
	{
		debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : AF Data size is less than ZCL Header. Bailing\r\n");
		return;

	}

	auto currentPtr = (uint8_t*)data.data();
	//auto currentPtr = data.data();
	ZclHeaderStruct header;
	std::memcpy((uint8_t*)&header,currentPtr,sizeof(header));

	//More Sanity Test here
	if(header.CommandIdentifier != ReadAttributesResponse)
	{
		debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : Unexpected Command Identifier Received. Bailing\r\n");
		return;
	}
	currentPtr += sizeof(header);
	auto remainingData = data.size() - sizeof(header);
	while(remainingData > 0)
	{
		auto currentIndex(0);
		//First two bytes gives Attribute identifier
		uint16_t attrId = Utility::convertData(currentPtr);
		currentPtr += sizeof(attrId);
		currentIndex += sizeof(attrId);
		//Next is the status
		DeviceStatusEnum status = (DeviceStatusEnum)*currentPtr;
		currentPtr += sizeof(status);
		currentIndex += sizeof(status);
		//If status is failed then move to next one
		if(status == STATUS_FAIL)
		{
			debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Status failed to get attribute\r\n");
			remainingData -= currentIndex;
			continue;
		}
		//We have the data now get it.
		auto dataType = (ClisterLibraryDataTypesEnum)*currentPtr;
		currentPtr += sizeof(status);
		currentIndex += sizeof(status);
		auto helper = ZclHelper();
		ZclDataType attributValue;
		auto attributeLength = helper.getZclData(dataType,currentPtr,attributValue);
		if(attributeLength == 0)
		{
			//We didn't get the data which needs to be investigated
			debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : Investigate why Attribute not received\r\n");
			return;
		}
		currentPtr += attributeLength;
		currentIndex += attributeLength;
		m_attributes[attrId] = attributValue;
		remainingData -= currentIndex;
	}
}


} /* namespace SimpleZigbeeName */
