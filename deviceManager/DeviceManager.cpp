/*
 * DeviceManager.cpp
 *
 *  Created on: 1 Oct 2019
 *      Author: hemant
 */
#include <functional>
#include "../../simpleDebug/SimpleDebug.h"
#include "../object/BaseObject.h"
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

void DeviceManager::start(std::shared_ptr<Observer> observer)
{
	m_observer = observer;

	//We will register for new devices detected.
	auto commandToregister = Utility::getAsyncyResponseCommand(SYNC_MT_ZDO_COMMAND0,ZDO_END_DEVICE_ANNCE_IND);
	using namespace std::placeholders;
	auto callbackNewDevice = std::bind(&DeviceManager::handleNewDevice,this,_1);
	//auto callbackNewDevice = std::bind(&DeviceManager::handleNewDevice,this);
	m_observer->subscribeAsynchCommand(commandToregister,callbackNewDevice);
}

void DeviceManager::handleNewDevice(std::unique_ptr<BaseObject>)
{
	SimpleDebugName::SimpleDebug::instance()->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Callback working\r\n");
}
DeviceManager::DeviceManager()
{
}

DeviceManager::~DeviceManager()
{
}

} /* namespace SimpleZigbeeName */
