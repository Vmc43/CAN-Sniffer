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
	MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const float Quartz_Speed, const uint_fast32_t Bitrate_CAN, const uint_fast16_t SPI_speed=500000);
	virtual ~MCP2515();

	virtual inline const uint_fast32_t GetBitRate()const{return Bitrate_CAN;}
	virtual inline const uint_fast32_t GetQuartz_Speed()const{return Quartz_Speed;}
	virtual inline const uint_fast8_t IsActive()const{return Active_Flag;}
	virtual void ChangeBitRate(const uint_fast16_t Bitrate);
	virtual void ChangeCLKoutPin(const uint_fast8_t OnOffFlag) const;
	virtual const uint_fast8_t Read_Rx_Status() const;
	//TODO Wakeup-Filter an und aus funktion (Schaltet Low-Pass filter an Rx leitung um versehentliches Aufwachen zu verhindern (in Sleepmode))
	//TODO FilterSetzfunktion() einmal für Standard und extended Übergabeparmeter sind (Filter,Maske==Filter)
	//TODO Filteraktivierfunktion(buffer,filter_von_buffer) (ectl als private funktion filtername direkt und andere fkt nur als switch-case)
	//TODO RXPufferInterruptein und aus funktion, Übergabe jeweiliger puffer (0,1)
	//TODO Universaler InterruptPin nur als RX-Interrupt setzen/ausmachen


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
	virtual void Reset() const;
	virtual void GoInConfigMode();
	virtual void GoInNormalMode();
	virtual void ChangeBitrateRegister(const uint_fast8_t CNF1_Value, const uint_fast8_t CNF2_Value, const uint_fast8_t CNF3_Value, const uint_fast16_t Bitrate);

	//Attribute:
	uint_fast32_t Bitrate_CAN=0;	//in kB/s
	uint_fast8_t Active_Flag=false;
	const float Quartz_Speed;		//in MHz
};

#endif /* MCP2515_H_ */
