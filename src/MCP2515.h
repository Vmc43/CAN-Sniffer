/*
 * MCP2515.h
 *
 *  Created on: 17.11.2016
 *      Author: test
 */

#ifndef MCP2515_H_
#define MCP2515_H_

#include "SPIDevice.h"

using namespace std;

class MCP2515:public SPIDevice
{
public:
	MCP2515(const unsigned int bus, const unsigned int device, const uint_fast16_t speed=500000);
	virtual ~MCP2515();
};

#endif /* MCP2515_H_ */
