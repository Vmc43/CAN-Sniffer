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
	MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const float Quartz_Speed, const uint_fast32_t Bitrate, const uint_fast16_t SPI_speed=500000);
	virtual ~MCP2515();

	virtual inline const uint_fast32_t GetBitRate()const{return Bitrate;}
	virtual inline const uint_fast32_t GetQuartz_Speed()const{return Quartz_Speed;}
	virtual inline const uint_fast8_t IsActive()const{return Active_Flag;}
	virtual void ChangeBitRate(const uint_fast32_t Bitrate);
	virtual void ChangeCLKoutPin(const uint_fast8_t OnOffFlag) const;

private:
	//Methoden:
	//Kommt von Mutterklasse, Chip kann aber nur 8-Bit Adressen
	virtual const int_fast8_t writeRegister(const uint_fast16_t registerAddress, const uint_fast8_t value) const;
	virtual const uint_fast8_t readRegister(const uint_fast16_t registerAddress) const;
	virtual const uint_fast8_t* readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress=0) const;
	//Spezialisierung nur für MCP2515
	virtual const int_fast8_t writeRegister(const uint_fast8_t registerAddress, const uint_fast8_t value) const;
	virtual const uint_fast8_t readRegister(const uint_fast8_t registerAddress) const;
	virtual const uint_fast8_t* readRegisters(const uint_fast8_t number, const uint_fast8_t fromAddress=0) const;
	//Eigene Funktionen
	virtual const uint_fast8_t Check_Value_Range(const uint_fast16_t number) const;
	virtual const int_fast8_t Bit_Modify(const uint_fast8_t adress, const uint_fast8_t mask, const uint_fast8_t data) const;
	virtual const int_fast8_t Init();

	//Attribute:
	uint_fast32_t Bitrate=0;	//in Bit/s
	uint_fast8_t Active_Flag=false;
	const float Quartz_Speed;	//in MHz
};

#endif /* MCP2515_H_ */
