/*
 * BaseObject.h
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_OBJECT_BASEOBJECT_H_
#define SRC_SIMPLEZIGBEE_OBJECT_BASEOBJECT_H_
#include <vector>
#include "stdint.h"
namespace SimpleZigbeeName {

class BaseObject
{
public:
	BaseObject() = default;
	virtual ~BaseObject() = default;
	//BaseObject(std::vector<uint8_t>&);
	virtual void create(std::vector<uint8_t>&) = 0;
	virtual void print() = 0;
};

}; /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_OBJECT_BASEOBJECT_H_ */
