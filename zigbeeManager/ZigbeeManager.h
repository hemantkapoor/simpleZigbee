/*
 * ZigbeeManager.h
 *
 *  Created on: 19 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_ZIGBEEMANAGER_ZIGBEEMANAGER_H_
#define SRC_SIMPLEZIGBEE_ZIGBEEMANAGER_ZIGBEEMANAGER_H_

#include <memory>

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName

{

	class ZigbeeComms;
	class Observer;

	class ZigbeeManager {
	public:
		ZigbeeManager() = delete;
		ZigbeeManager(std::shared_ptr<SimpleSerialName::Comms> comms);
		virtual ~ZigbeeManager();

		bool initialise();

	private:
		std::shared_ptr<ZigbeeComms> m_zigbeeComms;
		std::shared_ptr<Observer> m_observer;
	};

};

#endif /* SRC_SIMPLEZIGBEE_ZIGBEEMANAGER_ZIGBEEMANAGER_H_ */
