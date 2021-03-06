/*
 * MCP2515.h
 *
 *  Created on: 17.11.2016
 *      Author: test
 */

#ifndef MCP2515_H_
#define MCP2515_H_

#include "SPIDevice.h"
#include "GPIOPoll.h"

using namespace std;

class MCP2515:public SPIDevice
{
//Structs in Klasse:
public:
	struct CANMessage
	{
		uint_fast16_t id;
		uint_fast8_t rtr=false;
		uint_fast8_t length=0;
		uint_fast8_t data[8];
	};

	//Dummy
	virtual void Dummy();

public:
	MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast32_t Quartz_Speed_Hz, const uint_fast32_t Bitrate_CAN_Bit_s, const uint_fast8_t GlobalInterruptPinNr, const uint_fast32_t SPI_speed=500000);
	MCP2515(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast32_t Quartz_Speed_Hz, const uint_fast32_t Bitrate_CAN_Bit_s, const uint_fast8_t RxPuffer0Pin, const uint_fast8_t RxPuffer1Pin, const uint_fast32_t SPI_speed=500000);
	virtual ~MCP2515();

	virtual inline const uint_fast32_t GetBitRate()const{return Bitrate_CAN_Bit_s;}
	virtual inline const uint_fast32_t GetQuartz_Speed()const{return Quartz_Speed_Hz;}
	virtual inline const uint_fast8_t IsActive()const{return Active_Flag;}
	virtual void ChangeBitRate(const uint_fast32_t Bitrate_Bit_s);
	virtual void ChangeCLKoutPin(const uint_fast8_t OnOffFlag, const uint_fast8_t divider=1);
	virtual const uint_fast8_t Read_Rx_Status() const;
	virtual void SetFilterStandard(const uint_fast8_t FilterNr, const uint_fast16_t Filter=0);
	virtual void SetMaskStandard(const uint_fast8_t MaskNr, const uint_fast16_t Mask=0);
	virtual void SetFilterExtended(const uint_fast8_t FilterNr, const uint_fast32_t Filter=0);
	//virtual void SetMaskExtended(const uint_fast8_t MaskNr, const uint_fast16_t Mask=0);
	//TODO FilterSetzfunktion() einmal für Standard und extended Übergabeparmeter sind (FilterNr,Filter)
	//TODO MaskenSetzfunktion() einmal für Standard und extended Übergabeparmeter sind (MaskenNr,Maske)
	//TODO Filteraktivierfunktion(empfangsbuffer,filter_von_buffer,Welche Maske) (evtl als private funktion filtername direkt und andere fkt nur als switch-case)
	//TODO Init-Fkt() allgemein halten und dafür im Konstruktor nach Art der Initalisierung (Interruptpins) unterscheiden
	//TODO Konstruktor aufbohren (Interruptpins)
	//TODO Getter-Methoden für Flags???
	//TODO Sleepfunktion, oder besser erst wenn soweit erweitert das Pin für CAN-Transciver funzt

	//TODO Pollthread mit Callbackfkt bei Thread Erstellung
	//TODO Flag in welchen Modus (Unterscheidung durch Flag), 2x Pollthreadobjekt

	//Schönheit
	//TODO Rückgabewerte der Funktionen auf Sinnhaftigkeit prüfen (bei write und read fkt)

	//Infos:
	//TODO Einstellen ob nur Standard oder extended empfangen oder beides in RXM0 und RXM1 in RXB0CTRL bzw RXB1CTRL, welcher Filter für welchen Puffer ist dort auch
	//TODO Maske0 ist für Puffer0 und Maske1 ist für Puffer1
	//TODO SPI_Längenantwort immer wie in AVR umsetzen bei 2 byte senden und 2 byte empfangen Sendepuffer+2




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
	virtual const uint_fast8_t Check_Value_Range_16b_in_8b(const uint_fast16_t number) const;
	virtual const int_fast8_t Bit_Modify(const uint_fast8_t adress, const uint_fast8_t mask, const uint_fast8_t data) const;
	virtual void Init();
	virtual void Reset() const;
	virtual void GoInConfigMode();
	virtual void GoInNormalMode();
	virtual void ChangeBitrateRegister(const uint_fast8_t CNF1_Value, const uint_fast8_t CNF2_Value, const uint_fast8_t CNF3_Value, const uint_fast32_t Bitrate_CAN_Bit_s);
	virtual void SetInterruptPinOnlyForRecive(const uint_fast8_t state=true);
	virtual void Set_RXBx_Interrupt_Pin(const uint_fast8_t Buffer, const uint_fast8_t state=true);
	virtual void Set_Wake_Up_Filer(const uint_fast8_t state); //Schaltet Filter an CAN-Leitungen um versehentliches Aufwachen im sleppmode zu verhindern
	virtual const uint_fast8_t IsBool(const uint_fast8_t& number) const;
	virtual void Change_CLKOUT_PIN_REGISTER(const uint_fast8_t CLKEN_Bit, const uint_fast8_t CLKPRE) const;
	virtual int_fast8_t Send_Message(const CANMessage& Message) const;
	virtual const CANMessage Recive_Message(const uint_fast8_t Buffer) const;
	virtual const uint_fast8_t Bit_is_Set(const uint_fast8_t Wert, const uint_fast8_t BitNummer) const;
	virtual const uint_fast8_t Read_STATUS() const;
	virtual const uint_fast8_t TX_Puffer_ermitteln(const uint_fast8_t status) const;
	virtual const uint_fast8_t RX_Puffer_ermitteln(const uint_fast8_t status) const;
	virtual const uint_fast8_t Read_RX_STATUS() const;
	virtual const uint_fast8_t Get_RX_Array(const uint_fast8_t Befehl, uint_fast8_t Array[], const uint_fast8_t Laenge=14) const;
	virtual const CANMessage Get_Message_With_Buffer_Via_SPI() const;

	//Attribute:
	const uint_fast32_t Quartz_Speed_Hz=0;	//in Hz
	uint32_t Bitrate_CAN_Bit_s=0;		//in kB/s


	//Interrupt Flags
	uint_fast8_t Interrupt_Pin_Register=0;
	uint_fast8_t RXB0_InterruptPin_Flag=false;
	uint_fast8_t RXB1_InterruptPin_Flag=false;

	uint_fast8_t Wake_Up_Filter_Flag=false;

	uint_fast8_t Active_Flag=false;

	//CLK-Out:
	uint_fast8_t CLKOUT_Flag=true;
	uint_fast8_t Teiler_CLKOUT=8; //Siehe Datenblatt CANCTRL

	//Filter
	uint_fast32_t FilterMaskFilter0=0;
	uint_fast32_t FilterMaskFilter1=0;
	uint_fast32_t FilterMaskFilter2=0;
	uint_fast32_t FilterMaskFilter3=0;
	uint_fast32_t FilterMaskFilter4=0;
	uint_fast32_t FilterMaskFilter5=0;

	//Masken:
	uint_fast32_t Maske0=0;
	uint_fast32_t Maske1=0;

	//GPIO-Poll-Threads:
	const GPIO_Poll InterruptPin1;
	const GPIO_Poll InterruptPin2;
};

#endif /* MCP2515_H_ */
