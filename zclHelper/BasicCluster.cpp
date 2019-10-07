/*
 * BasicCluster.cpp
 *
 *  Created on: 4 Oct 2019
 *      Author: hemant
 */
#include "../../simpleSerial/comms/Comms.h"
#include "../../simpleDebug/SimpleDebug.h"
#include "../observer/Observer.h"
#include "../object/BaseObject.h"
#include "../utility/Utility.h"
#include "ZclHelper.h"
#include "BasicCluster.h"

namespace SimpleZigbeeName {

BasicCluster::BasicCluster(std::shared_ptr<Observer> observer, std::shared_ptr<SimpleSerialName::Comms> comms,
		uint16_t destAddress, uint8_t endPoint):
		m_observer(observer), m_comms(comms),
		m_destinationAddr(destAddress),
		m_endPoint(endPoint),
		m_transId(0)
{
}


BasicCluster::~BasicCluster()
{
}

std::vector<ZclDataType> BasicCluster::getAttributes(const std::vector<uint16_t>& attributes)
{
	auto returnValue = std::vector<ZclDataType>() ;
	ZclFrameControlFieldStruct frameControl{0x00};
	ZclHeaderStruct frameHeader{frameControl,0x01, AF_DATA_REQUEST};

	MtAfDataRequestCommandStruct commandData{m_destinationAddr,m_endPoint,m_endPoint,Basic_Cluster,m_transId, 0x30, 0x1E};
	commandData.Len = sizeof(ZclHeaderStruct) + (sizeof(uint16_t) *attributes.size());
	auto commandDataPtr = reinterpret_cast<uint8_t*>(&commandData);
	//HK Need to verify the endianess
	auto dataVector = std::vector<uint8_t>(commandDataPtr, commandDataPtr+sizeof(commandData));

	//Now we need to populate the AF Data
	auto afDataPtr = reinterpret_cast<uint8_t*>(&frameHeader);
	//HK Need to verify the endianess
	auto afdataVector = std::vector<uint8_t>(afDataPtr, afDataPtr+sizeof(frameHeader));
	auto attributesVector = Utility::convertData(attributes);
	std::copy(attributesVector.begin(),attributesVector.end(), std::back_inserter(afdataVector));

	//Finally copy the whole AF Data
	std::copy(afdataVector.begin(),afdataVector.end(), std::back_inserter(dataVector));


	auto getAtributesCommand =  Utility::constructMessage(SYNC_MT_AF_COMMAND0, AF_DATA_REQUEST,dataVector);

	auto syncResponseExpected = Utility::getSyncyResponseCommand(SYNC_MT_AF_COMMAND0, AF_DATA_REQUEST);
	m_observer->requestSyncResponse(syncResponseExpected);
	auto asyncResponseExpected = Utility::getAsyncyResponseCommand(SYNC_MT_AF_COMMAND0, AF_DATA_REQUEST);
	m_observer->requestSyncResponse(asyncResponseExpected);

	m_comms->transmitData(dataVector);

	auto respObject = m_observer->getSyncResponse(syncResponseExpected,std::chrono::seconds(1));
	auto debug = SimpleDebugName::SimpleDebug::instance();
	if(!respObject)
	{
		//Remove and return
		debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : No acknowledgement for Get Attributes\r\n");
		m_observer->removeRequestSyncResponse(syncResponseExpected);
		m_observer->removeRequestSyncResponse(asyncResponseExpected);
		return returnValue;
	}
	respObject->print();

	auto asyncrespObject = m_observer->getSyncResponse(asyncResponseExpected,std::chrono::seconds(1));
	if(!asyncrespObject)
	{
		//Remove and return
		debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : No data received for Get Attributes\r\n");
		m_observer->removeRequestSyncResponse(asyncResponseExpected);
		return returnValue;
	}

	//HK To be continued.

	return returnValue;
}

} /* namespace SimpleZigbeeName */
