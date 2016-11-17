/*
 * MCP2515.cpp
 *
 *  Created on: 17.11.2016
 *      Author: test
 */

#include "MCP2515.h"

MCP2515::MCP2515(const unsigned int bus, const unsigned int device, const uint_fast16_t speed)
	:SPIDevice(bus,device,speed)
{

}

MCP2515::~MCP2515()
{
	// TODO Auto-generated destructor stub
}
