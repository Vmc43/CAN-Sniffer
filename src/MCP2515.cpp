/*
 * MCP2515.cpp
 *
 *  Created on: 17.11.2016
 *      Author: test
 */

#include "MCP2515.h"
#include "mcp2515_defs.h"
#include <iostream>
#include <cstring> //Für memcpy() --> C-Style!

MCP2515::MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast16_t speed)
	:SPIDevice(bus,device,speed)
{

}

MCP2515::~MCP2515()
{
	// TODO Auto-generated destructor stub
}

const uint_fast8_t MCP2515::readRegister(const uint_fast8_t registerAddress) const
{
	const uint_fast8_t send[2]{SPI_READ,registerAddress};
	uint_fast8_t recive[2]{0};
	uint_fast8_t Return=0xFF;

	if(transfer(send,recive,2)!=-1)
	{
		Return=recive[0];
	}
	return Return;
}

const uint_fast8_t* MCP2515::readRegisters(const uint_fast8_t number, const uint_fast8_t fromAddress) const
{
	const uint_fast8_t send[number+1]{SPI_READ,fromAddress};
	uint_fast8_t recive[number+1];
	uint_fast8_t* Return=NULL;

	if(transfer(send,recive,sizeof(send))!=-1)
	{
		Return = new uint_fast8_t[number];
		memcpy(Return,recive,number);
	}
	return Return;
}

const int_fast8_t MCP2515::writeRegister(const uint_fast8_t registerAddress, const uint_fast8_t value) const
{
	const uint_fast8_t send[3]={SPI_WRITE,registerAddress,value}; //Kommando, Adresse, Wert
	uint_fast8_t recive[3]; //Nur zu Dummy-Zwecken, es kommt nichts von Chip zurück

	return transfer(send,recive,3);
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

const uint_fast8_t MCP2515::readRegister(const uint_fast16_t registerAddress) const
{
	if(Check_Value_Range(registerAddress))
	{
		return readRegister(static_cast<uint_fast8_t>(registerAddress));
	}
	cerr<<"Fehler übergebene Registeradresse zu groß!"<<endl;
	return -1;
}

const uint_fast8_t* MCP2515::readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress) const
{
	if(Check_Value_Range(fromAddress))
	{
		return readRegisters(number,static_cast<uint_fast8_t>(fromAddress));
	}
	return NULL;
}
