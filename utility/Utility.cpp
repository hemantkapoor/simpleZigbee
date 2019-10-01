/*
 * Utility.cpp
 *
 *  Created on: 17 Sep 2019
 *      Author: root
 */
#include "../../simpleDebug/SimpleDebug.h"
#include "../../simpleSerial/comms/Comms.h"
#include "../observer/Observer.h"
#include "../object/BaseObject.h"


#include "../object/mtZdo/async/MtZdoAsyncNodeDescResponse.h"
#include "../object/mtZdo/async/MtZdoAsyncActiveEPResponse.h"
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


std::unique_ptr<MtZdoAsyncNodeDescResponse> Utility::getNodeDescription(uint16_t destinationAddress,uint16_t networkAddress,std::shared_ptr<SimpleSerialName::Comms> comms,std::shared_ptr<Observer> observer)
{
	auto retVal = std::make_unique<MtZdoAsyncNodeDescResponse>();
	auto responseCommandExpected = getSyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_NODE_DESC_REQ);
	observer->requestSyncResponse(responseCommandExpected);

	auto asyncResponseExpected = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_NODE_DESC_REQ);
	observer->requestSyncResponse(asyncResponseExpected);

	auto dataTosend =  Utility::constructMessage(SYNC_MT_ZDO_COMMAND0,ZDO_NODE_DESC_REQ,
			MessageDataType{(uint8_t)(destinationAddress & 0xFF), (uint8_t)((destinationAddress & 0xFF00) >> 8),
			(uint8_t)(networkAddress & 0xFF), (uint8_t)((networkAddress & 0xFF00) >> 8)
			});
	comms->transmitData(dataTosend);
	auto respObject = observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));

	auto debug = SimpleDebugName::SimpleDebug::instance();
	if(!respObject)
	{
		//Remove and return
		debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No acknowledgement for Node Description\r\n");
		observer->removeRequestSyncResponse(responseCommandExpected);
		return retVal;
	}
	respObject->print();

	auto asyncrespObject = observer->getSyncResponse(asyncResponseExpected,std::chrono::seconds(1));
	if(!asyncrespObject)
	{
		//Remove and return
		debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No data received for Node Description\r\n");
		observer->removeRequestSyncResponse(asyncResponseExpected);
		return retVal;
	}
	asyncrespObject->print();

	auto getAsyncNodeDescObj = dynamicConvert<BaseObject,MtZdoAsyncNodeDescResponse>(std::move(asyncrespObject));
	if(getAsyncNodeDescObj)
	{
		retVal = std::move(getAsyncNodeDescObj);
	}


	return retVal;
}

std::pair<bool,std::vector<uint8_t>> Utility::getActiveEndPoints(uint16_t destinationAddress,uint16_t networkAddress,std::shared_ptr<SimpleSerialName::Comms> comms,std::shared_ptr<Observer> observer)
{
	std::vector<uint8_t> retVector{};
	auto debug = SimpleDebugName::SimpleDebug::instance();
	auto responseCommandExpected = Utility::getSyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_ACTIVE_EP_REQ);
	observer->requestSyncResponse(responseCommandExpected);

	auto asyncResponseExpected = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_ACTIVE_EP_REQ);
	observer->requestSyncResponse(asyncResponseExpected);

	auto dataTosend =  Utility::constructMessage(SYNC_MT_ZDO_COMMAND0,ZDO_ACTIVE_EP_REQ,
			MessageDataType{(uint8_t)(destinationAddress & 0xFF), (uint8_t)((destinationAddress & 0xFF00) >> 8),
			(uint8_t)(networkAddress & 0xFF), (uint8_t)((networkAddress & 0xFF00) >> 8)
			});
	comms->transmitData(dataTosend);
	auto respObject = observer->getSyncResponse(responseCommandExpected,std::chrono::seconds(1));
	if(!respObject)
	{
		//Remove and return
		debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No acknowledgement for Active End Point\r\n");
		observer->removeRequestSyncResponse(responseCommandExpected);
		return std::make_pair(false,retVector);
	}
	respObject->print();

	auto asyncrespObject = observer->getSyncResponse(asyncResponseExpected,std::chrono::seconds(1));
	if(!asyncrespObject)
	{
		//Remove and return
		debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : No data received for Active End Point\r\n");
		observer->removeRequestSyncResponse(asyncResponseExpected);
		return std::make_pair(false,retVector);
	}
	asyncrespObject->print();

	//Convert to get Active End points Object
	auto getActiveEndPoints = dynamicConvert<BaseObject,MtZdoAsyncActiveEPResponse>(std::move(asyncrespObject));
	if(getActiveEndPoints)
	{
		retVector = getActiveEndPoints->getEndPoints();
		return std::make_pair(true,retVector);
	}
	else
	{
		debug->log(SimpleDebugName::CRITICAL_WARNING, std::string(__PRETTY_FUNCTION__) + " : Error converting to dynamic object\r\n");
	}
	return std::make_pair(false,retVector);
}
