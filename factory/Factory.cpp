/*
 * Factory.cpp
 *
 *  Created on: 20 Sep 2019
 *      Author: hemant
 */
#include <iostream>
#include "../../simpleSerial/utility/Utility.h"
#include "../object/BaseObject.h"
#include "Factory.h"

using namespace SimpleZigbeeName;


std::unique_ptr<BaseObject> Factory::create(const std::vector<uint8_t>& data)
{
	//For now there is NO object that can be created
	std::cout << __PRETTY_FUNCTION__<< " : Nothing to handle below data "<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<< " : " << SimpleSerialName::Utility::hexStr(data.data(),data.size())<<std::endl;

	return std::unique_ptr<BaseObject>{};
}
