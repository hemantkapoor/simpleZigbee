/*
 * Utility.h
 *
 *  Created on: 17 Sep 2019
 *      Author: root
 */

#ifndef UTILITY_UTILITY_H_1
#define UTILITY_UTILITY_H_1

#include <stdint.h>
#include <string>
#include <vector>
#include <utility>
#include <memory>

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
using MessageDataType = std::vector<uint8_t>;
const uint8_t CLEAR_TYPE_MASK = 0x1F;
const uint8_t  SYNC_RESPONSE_CMD0_MASK = 0x40;
const uint8_t  SYNC_RESPONSE_CMD1_MASK = 0x80;

class Observer;
class MtZdoAsyncNodeDescResponse;
class MtZdoAsyncSimpleDescResponse;
class Utility
{
public:
	//Static class No constructor move or copy
	Utility() = delete;
	Utility& operator=(const Utility& ) = delete;
	Utility (const Utility &) = delete;
	~Utility() = delete;
	static uint8_t checksum(const MessageDataType& data);
	static MessageDataType constructMessage(uint8_t cmd0, uint8_t cmd1, const std::vector<uint8_t>& data = std::vector<uint8_t>{});
	static uint16_t getSyncyResponseCommand(uint8_t cmd0, uint8_t cmd1);
	static uint16_t getAsyncyResponseCommand(uint8_t cmd0, uint8_t cmd1);
	static std::vector<uint8_t> getBigEndian(uint16_t data);
	static std::vector<uint8_t> convertData(const std::vector<uint16_t>&);
	static std::vector<uint16_t> convertData(const std::vector<uint8_t>&);
	static uint16_t convertData(const uint8_t*);

	template<typename baseType, typename derivedType>
	static std::unique_ptr<derivedType> dynamicConvert(std::unique_ptr<baseType> baseObj)
	{
		auto tmp = dynamic_cast<derivedType*>(baseObj.get());
		std::unique_ptr<derivedType> derivedPointer;
		if(tmp != nullptr)
		{
			baseObj.release();
		    derivedPointer.reset(tmp);
		}
		return derivedPointer;
	}


	//General Helper functions
	static std::unique_ptr<MtZdoAsyncNodeDescResponse> getNodeDescription(uint16_t destinationAddress,uint16_t networkAddress,std::shared_ptr<SimpleSerialName::Comms>,std::shared_ptr<Observer>);
	static std::pair<bool,std::vector<uint8_t>> getActiveEndPoints(uint16_t destinationAddress,uint16_t networkAddress,std::shared_ptr<SimpleSerialName::Comms>,std::shared_ptr<Observer>);
	static std::unique_ptr<MtZdoAsyncSimpleDescResponse> getSimpleDescription(uint16_t destinationAddress,uint16_t networkAddress,uint8_t endPoint, std::shared_ptr<SimpleSerialName::Comms>,std::shared_ptr<Observer>);


};

};//End of namespace

#endif /* UTILITY_UTILITY_H_ */
