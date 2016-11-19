/*
 * MCP2515.cpp
 *
 *  Created on: 17.11.2016
 *      Author: test
 */

#include "MCP2515.h"
#include "mcp2515_defs.h"
#include <iostream>

MCP2515::MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast16_t speed)
	:SPIDevice(bus,device,speed)
{

}

MCP2515::~MCP2515()
{
	// TODO Auto-generated destructor stub
}

const uint_fast8_t MCP2515::readRegister(const uint_fast16_t registerAddress) const
{

}

const uint_fast8_t* MCP2515::readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress) const
{

}

const int_fast8_t MCP2515::writeRegister(const uint_fast8_t registerAddress, const uint_fast8_t value) const
{
	uint_fast8_t send[3];
	uint_fast8_t recive[3]; //Nur zu Dummy-Zwecken, es kommt nichts von Chip zurück

	//Befüllen von send[]
	send[0]=SPI_WRITE; 		//Kommando
	send[1]=registerAddress;//Adresse
	send[2]=value;			//Daten

	int_fast8_t Return=transfer(send,recive,3);

	return Return;
}

const int_fast8_t MCP2515::writeRegister(const uint_fast16_t registerAddress, const uint_fast8_t value) const
{
	if(Check_Value_Range(registerAddress))
	{
		return writeRegister(static_cast<uint_fast8_t>(registerAddress),value);
	}
	cerr<<"Fehler übergebene Registeradresse zu groß!"<<endl;
	return -1;
}

//Prüfung ob 16-Bit Int in 8-bit passt.
const uint_fast8_t MCP2515::Check_Value_Range(const uint_fast16_t number) const
{
	if(number<=0xFF)
	{
		return true;
	}
	return false;
}

const uint_fast8_t MCP2515::readRegister(const uint_fast8_t registerAddress) const
{
	if(Check_Value_Range(registerAddress))
	{
		return readRegister(static_cast<uint_fast8_t>(registerAddress));
	}
	cerr<<"Fehler übergebene Registeradresse zu groß!"<<endl;
	return -1;
}
