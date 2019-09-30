/*
 * Utility.cpp
 *
 *  Created on: 17 Sep 2019
 *      Author: root
 */
#include "Utility.h"

using namespace SimpleZigbeeName;

uint8_t Utility::checksum(const MessageDataType& data)
{
	uint8_t fcs(0);
	for (auto val : data)
	{
		fcs = fcs ^ val;
	}
	return fcs;
}


MessageDataType Utility::constructMessage(uint8_t cmd0, uint8_t cmd1, const std::vector<uint8_t>& data /*= std::vector<uint8_t>{}*/)
{
	//Lets work back wards
	MessageDataType returnData{data};
	//Lets add the length first
	auto startIt = returnData.begin();
	returnData.insert(startIt,cmd1);
	startIt = returnData.begin();
	returnData.insert(startIt,cmd0);
	uint8_t len = data.size();
	startIt = returnData.begin();
	returnData.insert(startIt,len);
	//No we calculate checksum
	auto fcs = Utility::checksum(returnData);
	returnData.push_back(fcs);
	startIt = returnData.begin();
	returnData.insert(startIt,0xFE);
	return returnData;
}

uint16_t Utility::getSyncyResponseCommand(uint8_t cmd0, uint8_t cmd1)
{
	return ((uint16_t(cmd0) | 0x0060) << 8) | cmd1;
}

uint16_t Utility::getAsyncyResponseCommand(uint8_t cmd0, uint8_t cmd1)
{
	return ( ((uint16_t ((cmd0) & CLEAR_TYPE_MASK) |  SYNC_RESPONSE_CMD0_MASK)<< 8) | (cmd1 | SYNC_RESPONSE_CMD1_MASK));
}

std::vector<uint8_t> Utility::getBigEndian(uint16_t data)
{
	std::vector<uint8_t> retVal{(uint8_t)(data & 0xFF), (uint8_t)((data & 0xFF00) >> 8)};
	return retVal;
}
