/*
 * BasicCluster.h
 *
 *  Created on: 4 Oct 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_ZCLHELPER_BASICCLUSTER_H_
#define SRC_SIMPLEZIGBEE_ZCLHELPER_BASICCLUSTER_H_

#include <memory>

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName
{
class Observer;

class BasicCluster {
public:
	BasicCluster() = delete;
	BasicCluster(std::shared_ptr<Observer>, std::shared_ptr<SimpleSerialName::Comms> comms, uint8_t endPoint);
	virtual ~BasicCluster();
};

} /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_ZCLHELPER_BASICCLUSTER_H_ */
