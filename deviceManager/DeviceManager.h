/*
 * DeviceManager.h
 *
 *  Created on: 1 Oct 2019
 *      Author: hemant
 */

#pragma once

#include<memory>

namespace SimpleSerialName
{
	class Comms;
};


namespace SimpleZigbeeName
{

class Observer;

using ListOfClusterType = std::vector<uint16_t>;

using ListOfInputandOutClusterType = std::pair<ListOfClusterType,ListOfClusterType>;

using DeviceEndPointsAndClusterType = std::map<uint8_t,ListOfInputandOutClusterType>;

class DeviceManager
{
public:
	virtual ~DeviceManager();
	//Singleton Pattern
	DeviceManager(const DeviceManager &other) = delete;
	DeviceManager(DeviceManager &&other) = delete;
	DeviceManager& operator=(const DeviceManager &other) = delete;
	DeviceManager& operator=(DeviceManager &&other) = delete;

	static DeviceManager* instance();
	void start(std::shared_ptr<Observer>, std::shared_ptr<SimpleSerialName::Comms> comms);

	void handleNewDevice(std::unique_ptr<BaseObject>);
	//void handleNewDevice(int);
private:
	DeviceManager();

	static DeviceManager* m_instance;
	std::shared_ptr<Observer> m_observer;
	std::shared_ptr<SimpleSerialName::Comms> m_comms;
};

} /* namespace SimpleZigbeeName */


