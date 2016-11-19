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
	MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast16_t speed=500000);
	virtual ~MCP2515();
private:
	//Kommt von Mutterklasse, Chip kann aber nur 8-Bit Adressen
	virtual const int_fast8_t writeRegister(const uint_fast16_t registerAddress, const uint_fast8_t value) const;
	virtual const uint_fast8_t readRegister(const uint_fast16_t registerAddress) const;
	virtual const uint_fast8_t* readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress=0) const;
	//Spezialisierung nur für MCP2515
	virtual const int_fast8_t writeRegister(const uint_fast8_t registerAddress, const uint_fast8_t value) const;
	virtual const uint_fast8_t readRegister(const uint_fast8_t registerAddress) const;
	//Eigene Funktionen
	virtual const uint_fast8_t Check_Value_Range(const uint_fast16_t number) const;
};

#endif /* MCP2515_H_ */
