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
#include <atomic>
#include <condition_variable>
#include <chrono>

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
	std::unique_ptr<BaseObject> getSyncResponse(uint16_t command,const std::chrono::seconds& duration);
	void removeRequestSyncResponse(uint16_t command);
private:
	std::shared_ptr<SimpleSerialName::Comms> m_comms;

	std::vector<uint16_t> m_syncRequestResponseList;
	std::mutex m_syncRequestResponseListMutex;

	std::map<uint16_t,std::unique_ptr<BaseObject>> m_syncResponseMap;
	std::mutex m_syncResponseMapMutex;

	std::condition_variable m_condVar;
	std::mutex m_condVarMutex;
	std::atomic<bool> m_dataAvailable = false;

	SimpleDebugName::SimpleDebug* m_debug;
};

}

#endif /* SRC_SIMPLEZIGBEE_OBSERVER_OBSERVER_H_ */
