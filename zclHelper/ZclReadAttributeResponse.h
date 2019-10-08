/*
 * ZclReadAttributeResponse.h
 *
 *  Created on: 8 Oct 2019
 *      Author: hemant
 */

#pragma once

#include <stdint.h>
#include <map>
#include "ZclHelper.h"

namespace SimpleZigbeeName
{

class ZclReadAttributeResponse
{
public:
	ZclReadAttributeResponse() = delete;
	ZclReadAttributeResponse(const std::vector<uint8_t>&);
	virtual ~ZclReadAttributeResponse() = default;
	std::map<uint16_t, ZclDataType> getAttributes() const { return m_attributes; }

private:
	//Map with key = Attribute identifier and Value = Attribute value
	std::map<uint16_t, ZclDataType> m_attributes;
};

} /* namespace SimpleZigbeeName */


