/*
 * Observer.h
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBSERVER_OBSERVER_H_
#define SRC_SIMPLEZIGBEE_OBSERVER_OBSERVER_H_
#include <vector>
#include <map>
#include <mutex>
#include<memory>

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleDebugName
{
	class SimpleDebug;
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

	void requestSyncResponse(uint16_t command);
	std::unique_ptr<BaseObject> getSyncResponse(uint16_t command);
	void removeRequestSyncResponse(uint16_t command);
private:
	std::shared_ptr<SimpleSerialName::Comms> m_comms;

	std::vector<uint16_t> m_syncRequestResponseList;
	std::mutex m_syncRequestResponseListMutex;

	std::map<uint16_t,std::unique_ptr<BaseObject>> m_syncResponseMap;
	std::mutex m_syncResponseMapMutex;

	SimpleDebugName::SimpleDebug* m_debug;
};

}

#endif /* SRC_SIMPLEZIGBEE_OBSERVER_OBSERVER_H_ */
