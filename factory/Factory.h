/*
 * Factory.h
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */

#ifndef SRC_SIMPLEZIGBEE_FACTORY_FACTORY_H_
#define SRC_SIMPLEZIGBEE_FACTORY_FACTORY_H_

#include "stdint.h"
#include <vector>
#include<memory>

namespace SimpleZigbeeName
{

class BaseObject;

class Factory {
public:
	Factory() = default;
	virtual ~Factory() = default;
	std::unique_ptr<BaseObject> create(const std::vector<uint8_t>& data);
};

}; /* namespace SimpleZigbeeName */

#endif /* SRC_SIMPLEZIGBEE_FACTORY_FACTORY_H_ */
