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
#include <chrono>
#include <thread>

MCP2515::MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const float Quartz_Speed, const uint_fast32_t Bitrate_CAN, const uint_fast16_t SPI_speed)
	:SPIDevice(bus,device,SPI_speed),
	 Quartz_Speed(Quartz_Speed)
{
	MCP2515::Bitrate_CAN=Bitrate_CAN;
	Init();
}

MCP2515::~MCP2515()
{
	// TODO Auto-generated destructor stub
}

const uint_fast8_t MCP2515::readRegister(const uint_fast8_t registerAddress) const
{
	const uint_fast8_t send[2]{SPI_READ,registerAddress};
	uint_fast8_t receive[2];
	uint_fast8_t Return=0xFF;

	if(transfer(send,receive,2)!=-1)
	{
		Return=receive[0];
	}
	return Return;
}

const uint_fast8_t* MCP2515::readRegisters(const uint_fast8_t number, const uint_fast8_t fromAddress) const
{
	const uint_fast8_t send[number+1]{SPI_READ,fromAddress};
	uint_fast8_t receive[number+1];
	uint_fast8_t* Return=NULL;

	if(transfer(send,receive,sizeof(send))!=-1)
	{
		Return = new uint_fast8_t[number];
		memcpy(Return,receive,number);
	}
	return Return;
}

const int_fast8_t MCP2515::writeRegister(const uint_fast8_t registerAddress, const uint_fast8_t value) const
{
	const uint_fast8_t send[3]={SPI_WRITE,registerAddress,value}; //Kommando, Adresse, Wert
	uint_fast8_t receive[3]; //Nur zu Dummy-Zwecken, es kommt nichts von Chip zurück

	return transfer(send,receive,3);
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

const int_fast8_t MCP2515::Bit_Modify(const uint_fast8_t adress, const uint_fast8_t mask, const uint_fast8_t data) const
{
	const uint_fast8_t send[4]={SPI_BIT_MODIFY,adress,mask,data};
	uint_fast8_t receive[4];

	return transfer(send,receive,4);
}

const int_fast8_t MCP2515::Init()
{
	Reset();
	ChangeBitRate(Bitrate_CAN);
	SetInterruptPinOnlyForRecive();
}

void MCP2515::ChangeBitRate(const uint_fast16_t Bitrate)
{
	if(Bitrate>1000)
	{
		cerr<<"Baudrate zu groß!"<<endl;
		return;
	}

	uint_fast8_t Active_Flag_vorher=false;
	if(Active_Flag)
	{
		GoInConfigMode();
		Active_Flag_vorher=true;
	}

	//Unterscheidung nach verschiedenen Quarzen...
	if(Quartz_Speed==8)
	{
		//Berechnung: https://www.kvaser.com/support/calculators/bit-timing-calculator/
		switch(Bitrate)
			{
			case 25:
				ChangeBitrateRegister(0x07,0xB6,0x04,Bitrate);
				break;
			case 125:
				ChangeBitrateRegister(0x01,0xAC,0x03,Bitrate);
				break;
			case 250:
				ChangeBitrateRegister(0x00,0xAC,0x03,Bitrate);
				break;
			case 500:
				ChangeBitrateRegister(0x00,0x91,0x01,Bitrate);
				break;
			case 1000:
				cerr<<"1 MB/s ist mit diesen Quarz nicht möglich!"<<endl;
				break;
			default:
				cerr<<"Ungültige Baudrate eingestellt es geht nur 25,125,250 und 500kB/s"<<endl;
				break;
			}
	}
	else if(Quartz_Speed==16)
	{
		switch(Bitrate)
			{
			case 25:
				ChangeBitrateRegister(0x0F,0xB6,0x04,Bitrate);
				break;
			case 125:
				ChangeBitrateRegister(0x03,0xAC,0x03,Bitrate);
				break;
			case 250:
				ChangeBitrateRegister(0x01,0xAC,0x03,Bitrate);
				break;
			case 500:
				ChangeBitrateRegister(0x00,0xAC,0x03,Bitrate);
				break;
			case 1000:
				ChangeBitrateRegister(0x00,0x91,0x01,Bitrate);
				break;
			default:
				cerr<<"Ungültige Baudrate eingestellt es geht nur 25,125,250,500 und 1000kB/s"<<endl;
				break;
			}
	}

	if(Active_Flag_vorher)
	{
		GoInNormalMode();
	}
}

void MCP2515::ChangeCLKoutPin(const uint_fast8_t OnOffFlag) const
{

}

const uint_fast8_t MCP2515::Read_Rx_Status() const
{
	return readRegister((uint_fast8_t)SPI_RX_STATUS);
}

void MCP2515::Reset() const
{
	SPIDevice::write(SPI_RESET);
	//Warten bis Reset vorbei
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void MCP2515::GoInConfigMode()
{
	Bit_Modify(CANCTRL,0xE0,0x04);
	Active_Flag=false;
}

void MCP2515::GoInNormalMode()
{
	Bit_Modify(CANCTRL,0xE0,0x00);
	Active_Flag=true;
}

void MCP2515::ChangeBitrateRegister(const uint_fast8_t CNF1_Value, const uint_fast8_t CNF2_Value, const uint_fast8_t CNF3_Value, const uint_fast16_t Bitrate)
{
	writeRegister((uint_fast8_t)CNF1,CNF1_Value);
	writeRegister((uint_fast8_t)CNF2,CNF2_Value);
	Bit_Modify(CNF3,0x07,CNF3_Value); //Nur letzten 3 Bit ändern
	Bitrate_CAN=Bitrate;
}

void MCP2515::SetInterruptPinOnlyForRecive(const uint_fast8_t state)
{
	if(state==true)
	{
		SetInterruptPinRegister(3);
	}
	else
	{
		SetInterruptPinRegister(0);
	}
}

void MCP2515::SetInterruptPinRegister(const uint_fast8_t Value)
{
	writeRegister((uint_fast8_t)CANINTE,Value);
	Interrupt_Pin_Register=Value;
}

void MCP2515::Set_RXBx_Interrupt_Pin(const uint_fast8_t Buffer, const uint_fast8_t state)
{
	if(Buffer!=0||Buffer!=1)
	{
		cerr<<"Set_RXBx_Interrupt_Pin falschen Puffer übergeben!"<<endl;
		return;
	}

	if(Buffer==0)
	{
		if(state==true)
		{
			Bit_Modify(BFPCTRL,5,5);
			RXB0_InterruptPin_Flag=true;
		}
		else
		{
			Bit_Modify(BFPCTRL,5,0);
			RXB0_InterruptPin_Flag=false;
		}
	}
	else //Buffer 1
	{
		if(state==true)
		{
			Bit_Modify(BFPCTRL,10,10);
			RXB1_InterruptPin_Flag=true;
		}
		else
		{
			Bit_Modify(BFPCTRL,10,0);
			RXB1_InterruptPin_Flag=false;
		}
	}
}

void MCP2515::Set_Wake_Up_Filer(const uint_fast8_t state)
{
	if(state==true)
	{
		Bit_Modify(CNF3,64,64);
		Wake_Up_Filter_Flag=true;
	}
	else
	{
		Bit_Modify(CNF3,64,0);
		Wake_Up_Filter_Flag=false;
	}
}
