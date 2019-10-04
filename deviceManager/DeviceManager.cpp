/*
 * DeviceManager.cpp
 *
 *  Created on: 1 Oct 2019
 *      Author: hemant
 */
#include <sstream>
#include <functional>
#include "../../simpleDebug/SimpleDebug.h"
#include "../object/BaseObject.h"
#include "../object/mtZdo/async/MtZdoAsyncEndDeviceAnnceInd.h"
#include "../object/mtZdo/async/MtZdoAsyncNodeDescResponse.h"
#include "../object/mtZdo/async/MtZdoAsyncActiveEPResponse.h"
#include "../object/mtZdo/async/MtZdoAsyncSimpleDescResponse.h"
#include "../utility/Utility.h"
#include "../observer/Observer.h"
#include "DeviceManager.h"

namespace SimpleZigbeeName {

DeviceManager* DeviceManager::m_instance = nullptr;

DeviceManager* DeviceManager::instance()
{
	if(m_instance == nullptr)
	{
		m_instance = new DeviceManager();
	}
	return m_instance;
}

void DeviceManager::start(std::shared_ptr<Observer> observer, std::shared_ptr<SimpleSerialName::Comms> comms)
{
	m_observer = observer;
	m_comms = comms;

	//We will register for new devices detected.
	auto commandToregister = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_END_DEVICE_ANNCE_IND);
	using namespace std::placeholders;
	auto callbackNewDevice = std::bind(&DeviceManager::handleNewDevice,this,_1);
	//auto callbackNewDevice = std::bind(&DeviceManager::handleNewDevice,this);
	m_observer->subscribeAsynchCommand(commandToregister,callbackNewDevice);
}

void DeviceManager::handleNewDevice(std::unique_ptr<BaseObject> obj)
{
	auto debug = SimpleDebugName::SimpleDebug::instance();
	debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Callback working\r\n");
	obj->print();
	auto newDevInfo = Utility::dynamicConvert<BaseObject,MtZdoAsyncEndDeviceAnnceInd>(std::move(obj));
	if(!newDevInfo)
	{
		debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot convert to End Dev Object\r\n");
		return;
	}
	auto data = newDevInfo->getData();

	//Lets enquire more details about the new device
	auto nodeDescriptionObject = Utility::getNodeDescription(data.SrcAddr,data.NwkAddr,m_comms,m_observer);

	if(!nodeDescriptionObject)
	{
		debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : Failed to get Node Description of the device\r\n");
		return;
	}
	nodeDescriptionObject->print();

	//Lets try to get the active end points now
	auto [result, activeEndPoint] = Utility::getActiveEndPoints(data.SrcAddr,data.NwkAddr,m_comms,m_observer);

	if(result == false)
	{
		debug->log(SimpleDebugName::ERROR, std::string(__PRETTY_FUNCTION__) + " : Failed to get Active End points of the device\r\n");
		return;
	}
	//For now just print
	std::stringstream outputSting;
	for(auto endPoint : activeEndPoint)
	{
		outputSting << "\r\nSimple Descriptor for Endpoint 0x" << std::hex << (int)endPoint <<" are \r\n";
		debug->log(SimpleDebugName::LOG, outputSting);
		outputSting.str(std::string());
		//Also lets get Simple Descriptor for each end points and print it
		{
			auto simpleDescObj = Utility::getSimpleDescription(data.SrcAddr,data.NwkAddr,endPoint,m_comms,m_observer);
			if(simpleDescObj)
			{
				simpleDescObj->print();

				//Let us get details for every input cluster.
			}
			else
			{
				outputSting.str(std::string());
				outputSting << "\r\nDidn't get Simple descriptor for Endpoint 0x" << std::hex << endPoint <<" are \r\n";
				debug->log(SimpleDebugName::ERROR, outputSting);
				outputSting.str(std::string());
			}
		}
	}

}
DeviceManager::DeviceManager()
{
}

DeviceManager::~DeviceManager()
{
}

} /* namespace SimpleZigbeeName */
