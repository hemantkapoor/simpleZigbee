/*
 * DeviceManager.h
 *
 *  Created on: 1 Oct 2019
 *      Author: hemant
 */

#pragma once

#include<memory>


namespace SimpleZigbeeName
{

class Observer;

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
	void start(std::shared_ptr<Observer>);

	void handleNewDevice(std::unique_ptr<BaseObject>);
	//void handleNewDevice(int);
private:
	DeviceManager();

	static DeviceManager* m_instance;
	std::shared_ptr<Observer> m_observer;
};

} /* namespace SimpleZigbeeName */


