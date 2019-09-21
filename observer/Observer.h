/*
 * Observer.h
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBSERVER_OBSERVER_H_
#define SRC_SIMPLEZIGBEE_OBSERVER_OBSERVER_H_

#include<memory>

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName
{

class BaseObject;

class Observer
{
public:
	Observer() = delete;

	Observer(const Observer&) = delete;
	Observer& operator=(const Observer&) = delete;


	Observer(std::shared_ptr<SimpleSerialName::Comms> comms);
	virtual ~Observer();
	void handleReceivedMessage(std::unique_ptr<BaseObject>);
private:
	std::shared_ptr<SimpleSerialName::Comms> m_comms;
};

}

#endif /* SRC_SIMPLEZIGBEE_OBSERVER_OBSERVER_H_ */
