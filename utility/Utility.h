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
#include <memory>

namespace SimpleZigbeeName
{
using MessageDataType = std::vector<uint8_t>;
const uint8_t CLEAR_TYPE_MASK = 0x1F;
const uint8_t  SYNC_RESPONSE_CMD0_MASK = 0x40;
const uint8_t  SYNC_RESPONSE_CMD1_MASK = 0x80;
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
};

};//End of namespace

#endif /* UTILITY_UTILITY_H_ */
