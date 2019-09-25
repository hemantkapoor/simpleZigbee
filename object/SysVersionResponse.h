/*
 * SysVersionResponse.h
 *
 *  Created on: 22 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_SYSVERSIONRESPONSE_H_
#define SRC_SIMPLEZIGBEE_OBJECT_SYSVERSIONRESPONSE_H_

#include "BaseObject.h"

namespace SimpleDebugName
{
	class SimpleDebug;
};

namespace SimpleZigbeeName {

struct __attribute__ ((packed)) VersionAttributeStruct
{
	uint8_t TransportRev; 	//Transport protocol revision
	uint8_t Product;		//Product Id
	uint8_t MajorRel;		//Software major release number
	uint8_t MinorRel;		//Software minor release number
	uint8_t MaintRel;		//Software maintenance release number
};

class SysVersionResponse: public BaseObject
{
public:
	SysVersionResponse();
	virtual ~SysVersionResponse();
	virtual bool create(const std::vector<uint8_t>&) final;
	virtual void print() final;
	virtual uint16_t getCommand() final { return m_command; }
private:
	VersionAttributeStruct m_version;
	bool m_dataPopulated = false;
	uint16_t m_command;
	SimpleDebugName::SimpleDebug* m_debug;
};

} /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_OBJECT_SYSVERSIONRESPONSE_H_ */
