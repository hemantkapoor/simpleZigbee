/*
 * BasicCluster.h
 *
 *  Created on: 4 Oct 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_ZCLHELPER_BASICCLUSTER_H_
#define SRC_SIMPLEZIGBEE_ZCLHELPER_BASICCLUSTER_H_

#include <map>
#include <memory>
#include "ZclHelper.h"

namespace SimpleSerialName
{
	class Comms;
};

namespace SimpleZigbeeName
{
class Observer;

enum BasicClusterAttributesEnum : uint16_t
{
	ZCLVERSION = 0x0000,
	APPLICATIONVERSION = 0x0001,
	STACKVERSION = 0x0002,
	HWVERSION = 0x0003,
	MANUFACTURERNAME = 0x0004,
	MODELIDENTIFIER = 0x0005,
	DATECODE = 0x0006,
	POWERSOURCE = 0x0007,
	LOCATIONDESCRIPTION = 0x0010,
	PHYSICALENVIRONMENT = 0x0011,
	DEVICEENABLED = 0x0012,
	ALARMMASK = 0x0013,
	DISABLELOCALCONFIG = 0x0014,
	SWBUILDID = 0x4000
};

class BasicCluster {
public:
	BasicCluster() = delete;
	BasicCluster(std::shared_ptr<Observer>, std::shared_ptr<SimpleSerialName::Comms> comms, uint16_t destAddress, uint8_t endPoint);
	virtual ~BasicCluster();
	std::map<uint16_t, ZclDataType> getAttributes(const std::vector<uint16_t>&);

private:
	std::shared_ptr<Observer> m_observer;
	std::shared_ptr<SimpleSerialName::Comms> m_comms;
	uint16_t m_destinationAddr;
	uint8_t m_endPoint;
	uint8_t m_transId;
};

} /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_ZCLHELPER_BASICCLUSTER_H_ */
