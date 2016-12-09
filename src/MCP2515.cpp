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

MCP2515::MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast32_t Quartz_Speed_Hz, const uint_fast32_t Bitrate_CAN_Bit_s,const uint_fast8_t GlobalInterruptPinNr, const uint_fast32_t SPI_speed)
	:SPIDevice(bus,device,SPI_speed),
	 Quartz_Speed_Hz(Quartz_Speed_Hz),
	 Bitrate_CAN_Bit_s(Bitrate_CAN_Bit_s),
	 InterruptPin1(this),
	 InterruptPin2(this)
{
	//MCP2515::Bitrate_CAN_Bit_s=Bitrate_CAN_Bit_s;
	Init();

	//Pinkonfiguration:
	SetInterruptPinOnlyForRecive();

	GoInNormalMode();
}

MCP2515::MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast32_t Quartz_Speed_Hz, const uint_fast32_t Bitrate_CAN_Bit_s, const uint_fast8_t RxPuffer0Pin, const uint_fast8_t RxPuffer1Pin, const uint_fast32_t SPI_speed)
	:SPIDevice(bus,device,SPI_speed),
	 Quartz_Speed_Hz(Quartz_Speed_Hz),
	 InterruptPin1(this),
	 InterruptPin2(this)
{
	MCP2515::Bitrate_CAN_Bit_s=Bitrate_CAN_Bit_s;
	Init();

	//Pinkonfiguration:
	Set_RXBx_Interrupt_Pin(0);	//Interrupt für Puffer 0 auf MCP2515 aktivieren
	Set_RXBx_Interrupt_Pin(1);	//Interrupt für Puffer 1 auf MCP2515 aktivieren

	GoInNormalMode();
}

MCP2515::~MCP2515()
{
	// TODO Auto-generated destructor stub
}

const uint_fast8_t MCP2515::readRegister(const uint_fast8_t registerAddress) const
{
	const uint_fast8_t send[3]{SPI_READ,registerAddress};
	uint_fast8_t receive[3];
	uint_fast8_t Return=0xFF;

	if(transfer(send,receive,3)!=-1)
	{
		Return=receive[2];
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
	if(Check_Value_Range_16b_in_8b(registerAddress))
	{
		return writeRegister(static_cast<uint_fast8_t>(registerAddress),value);
	}
	cerr<<"Fehler übergebene Registeradresse zu groß!"<<endl;
	return -1;
}

//Prüfung ob 16-Bit Int in 8-bit passt.
const uint_fast8_t MCP2515::Check_Value_Range_16b_in_8b(const uint_fast16_t number) const
{
	if(number<=0xFF)
	{
		return true;
	}
	return false;
}

const uint_fast8_t MCP2515::readRegister(const uint_fast16_t registerAddress) const
{
	if(Check_Value_Range_16b_in_8b(registerAddress))
	{
		return readRegister(static_cast<uint_fast8_t>(registerAddress));
	}
	cerr<<"Fehler übergebene Registeradresse zu groß!"<<endl;
	return -1;
}

const uint_fast8_t* MCP2515::readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress) const
{
	if(Check_Value_Range_16b_in_8b(fromAddress))
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

void MCP2515::Init()
{
	Reset();
	ChangeBitRate(Bitrate_CAN_Bit_s);

	//TODO Sauberes Init
	//Alle Filter aus:
	Bit_Modify(RXB0CTRL,0b1100000,0b1100000); //Buffer0
	Bit_Modify(RXB1CTRL,0b1100000,0b1100000); //Buffer1
}

void MCP2515::ChangeBitRate(const uint_fast32_t Bitrate_Bit_s)
{
	//Fehlerbehandlung
	if(Bitrate_Bit_s>1000000)
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
	if(Quartz_Speed_Hz==8000000)
	{
		//Berechnung: https://www.kvaser.com/support/calculators/bit-timing-calculator/
		switch(Bitrate_Bit_s)
			{
			case 25000:
				ChangeBitrateRegister(0x07,0xB6,0x04,Bitrate_CAN_Bit_s);
				break;
			case 125000:
				ChangeBitrateRegister(0x01,0xAC,0x03,Bitrate_CAN_Bit_s);
				break;
			case 250000:
				ChangeBitrateRegister(0x00,0xAC,0x03,Bitrate_CAN_Bit_s);
				break;
			case 500000:
				ChangeBitrateRegister(0x00,0x91,0x01,Bitrate_CAN_Bit_s);
				break;
			case 1000000:
				cerr<<"1 MB/s ist mit diesen Quarz nicht möglich!"<<endl;
				break;
			default:
				cerr<<"Ungültige Baudrate eingestellt es geht nur 25,125,250 und 500kB/s"<<endl;
				break;
			}
	}
	else if(Quartz_Speed_Hz==16000000)
	{
		switch(Bitrate_Bit_s)
			{
			case 25000:
				ChangeBitrateRegister(0x0F,0xB6,0x04,Bitrate_CAN_Bit_s);
				break;
			case 125000:
				ChangeBitrateRegister(0x03,0xAC,0x03,Bitrate_CAN_Bit_s);
				break;
			case 250000:
				ChangeBitrateRegister(0x01,0xAC,0x03,Bitrate_CAN_Bit_s);
				break;
			case 500000:
				ChangeBitrateRegister(0x00,0xAC,0x03,Bitrate_CAN_Bit_s);
				break;
			case 1000000:
				ChangeBitrateRegister(0x00,0x91,0x01,Bitrate_CAN_Bit_s);
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

void MCP2515::ChangeCLKoutPin(const uint_fast8_t OnOffFlag, const uint_fast8_t divider)
{
	if((divider!=1 || divider!=2 || divider!=4 || divider!=8) && IsBool(OnOffFlag))
	{
		cerr<<"Ungültigen Teiler an ChangeCLKoutPin() übergeben, darf nur 1,2,4 oder 8 sein!"<<endl;
		return;
	}

	//Geht nur im Config-mode
	uint_fast8_t Active_Flag_vorher=false;
	if(Active_Flag)
	{
		GoInConfigMode();
		Active_Flag_vorher=true;
	}

	switch(divider)
	{
		case 1:
			Change_CLKOUT_PIN_REGISTER(OnOffFlag,0b00),
			Teiler_CLKOUT=1;
			break;
		case 2:
			Change_CLKOUT_PIN_REGISTER(OnOffFlag,0b01),
			Teiler_CLKOUT=2;
			break;
		case 4:
			Change_CLKOUT_PIN_REGISTER(OnOffFlag,0b10),
			Teiler_CLKOUT=4;
			break;
		case 8:
			Change_CLKOUT_PIN_REGISTER(OnOffFlag,0b11),
			Teiler_CLKOUT=8;
			break;
		default:
			cerr<<"Ungültigen Teiler an ChangeCLKoutPin() übergeben, darf nur 1,2,4 oder 8 sein!"<<endl;
			break;
	}

	if(Active_Flag_vorher)
	{
		GoInNormalMode();
	}
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

void MCP2515::ChangeBitrateRegister(const uint_fast8_t CNF1_Value, const uint_fast8_t CNF2_Value, const uint_fast8_t CNF3_Value, const uint_fast32_t Bitrate_CAN_Bit_s)
{
	writeRegister((uint_fast8_t)CNF1,CNF1_Value);
	writeRegister((uint_fast8_t)CNF2,CNF2_Value);
	Bit_Modify(CNF3,0x07,CNF3_Value); //Nur letzten 3 Bit ändern
	MCP2515::Bitrate_CAN_Bit_s=Bitrate_CAN_Bit_s;
}

void MCP2515::SetInterruptPinOnlyForRecive(const uint_fast8_t state)
{
	if(state==true)
	{
		writeRegister((uint_fast8_t)CANINTE,3);
		Interrupt_Pin_Register=true;
	}
	else
	{
		writeRegister((uint_fast8_t)CANINTE,0);
		Interrupt_Pin_Register=false;
	}
}

void MCP2515::Set_RXBx_Interrupt_Pin(const uint_fast8_t Buffer, const uint_fast8_t state)
{
	if((Buffer!=0||Buffer!=1) && IsBool(state))
	{
		cerr<<"Set_RXBx_Interrupt_Pin falsche Nummern für Puffer übergeben!"<<endl;
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
	//Geht nur im Config-mode
	uint_fast8_t Active_Flag_vorher=false;
	if(Active_Flag)
	{
		GoInConfigMode();
		Active_Flag_vorher=true;
	}

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

	if(Active_Flag_vorher)
	{
		GoInNormalMode();
	}
}

void MCP2515::SetFilterStandard(const uint_fast8_t FilterNr, const uint_fast16_t Filter)
{
	if(!(FilterNr>=0 && FilterNr<=5))
	{
		cerr<<"Übergabewert FilterNr in SetFilterStandard muss zwischen 0 und 5 sein!"<<endl;
		return;
	}

	if(Filter>2048)
	{
		cerr<<"Filter darf nicht größer als 11bit (2048) sein!"<<endl;
		return;
	}

	//Geht nur im Config-mode
	uint_fast8_t Active_Flag_vorher=false;
	if(Active_Flag)
	{
		GoInConfigMode();
		Active_Flag_vorher=true;
	}

	//Registerwerte zusammenbasteln:
	const uint_fast8_t HighTeil = Filter >> 3;	//Untere 3 Bit nach rechts rausshifften
	const uint_fast8_t LowTeil = Filter << 5;		//Obere 5 Bits werden überschrieben und die unteren 3 Bits von Mask sind die oberen 3 eines 8 Bit Registers

	switch(FilterNr)
		{
			case 0:
				writeRegister((uint_fast8_t)RXF0SIDH,HighTeil);
				writeRegister((uint_fast8_t)RXF0SIDL,LowTeil);
				FilterMaskFilter0=Filter;
				break;
			case 1:
				writeRegister((uint_fast8_t)RXF1SIDH,HighTeil);
				writeRegister((uint_fast8_t)RXF1SIDL,LowTeil);
				FilterMaskFilter1=Filter;
				break;
			case 2:
				writeRegister((uint_fast8_t)RXF2SIDH,HighTeil);
				writeRegister((uint_fast8_t)RXF2SIDL,LowTeil);
				FilterMaskFilter2=Filter;
				break;
			case 3:
				writeRegister((uint_fast8_t)RXF3SIDH,HighTeil);
				writeRegister((uint_fast8_t)RXF3SIDL,LowTeil);
				FilterMaskFilter3=Filter;
				break;
			case 4:
				writeRegister((uint_fast8_t)RXF4SIDH,HighTeil);
				writeRegister((uint_fast8_t)RXF4SIDL,LowTeil);
				FilterMaskFilter4=Filter;
				break;
			case 5:
				writeRegister((uint_fast8_t)RXF5SIDH,HighTeil);
				writeRegister((uint_fast8_t)RXF5SIDL,LowTeil);
				FilterMaskFilter5=Filter;
				break;
			default:
				cerr<<"Ungültige Filternummer an SetFilterStandard übergeben, darf nur zwischen 0 und 5 sein"<<endl;
				break;
		}

	if(Active_Flag_vorher)
	{
		GoInNormalMode();
	}
}

void MCP2515::SetFilterExtended(const uint_fast8_t FilterNr, const uint_fast32_t Filter)
{
	if(!(FilterNr>=0 && FilterNr<=5))
	{
		cerr<<"Übergabewert FilterNr in SetFilterStandard muss zwischen 0 und 5 sein!"<<endl;
		return;
	}

	if(Filter>2048)
	{
		cerr<<"Filter darf nicht größer als 11bit (2048) sein!"<<endl;
		return;
	}

	//Geht nur im Config-mode
	uint_fast8_t Active_Flag_vorher=false;
	if(Active_Flag)
	{
		GoInConfigMode();
		Active_Flag_vorher=true;
	}

	//Eigentliches Blabla
	//TODO Extendedfilterfunktion

	if(Active_Flag_vorher)
	{
		GoInNormalMode();
	}
}

const uint_fast8_t MCP2515::IsBool(const uint_fast8_t& number) const
{
	if(number==true || number==false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MCP2515::Change_CLKOUT_PIN_REGISTER(const uint_fast8_t CLKEN_Bit, const uint_fast8_t CLKPRE) const
{
	//TODO Bei Reset Teilervariable wieder auf 8 Stellen
	if(IsBool(CLKEN_Bit))
	{
		uint_fast8_t Temp=CLKEN_Bit;
		Temp = Temp << 2;
		Temp |= CLKPRE;

		Bit_Modify(CANCTRL,7,Temp);
	}
	else
	{
		cerr<<"CLKEN_Bit in Change_CLKOUT_PIN_REGISTER() darf nur true oder false sein!"<<endl;
	}
}

void MCP2515::SetMaskStandard(const uint_fast8_t MaskNr, const uint_fast16_t Mask)
{
	if(MaskNr!=0 || MaskNr!=1)
	{
		cerr<<"Übergabewert MaskNr in SetFilterStandard darf nur 0 oder 1 sein!"<<endl;
		return;
	}

	if(Mask>2048)
	{
		cerr<<"Maske darf nicht größer als 11bit (2048) sein!"<<endl;
		return;
	}

	//Geht nur im Config-mode
	uint_fast8_t Active_Flag_vorher=false;
	if(Active_Flag)
	{
		GoInConfigMode();
		Active_Flag_vorher=true;
	}

	//Registerwerte zusammenbasteln:
	const uint_fast8_t HighTeil = Mask >> 3;	//Untere 3 Bit nach rechts rausshifften
	const uint_fast8_t LowTeil = Mask << 5;		//Obere 5 Bits werden überschrieben und die unteren 3 Bits von Mask sind die oberen 3 eines 8 Bit Registers

	if(MaskNr==0)
	{
		writeRegister((uint_fast8_t)RXM0SIDH,HighTeil);
		writeRegister((uint_fast8_t)RXM0SIDL,LowTeil);
		Maske0=Mask;
	}
	else
	{
		writeRegister((uint_fast8_t)RXM1SIDH,HighTeil);
		writeRegister((uint_fast8_t)RXM1SIDL,LowTeil);
		Maske1=Mask;
	}

	if(Active_Flag_vorher)
	{
		GoInNormalMode();
	}
}

void MCP2515::Dummy()
{

}
