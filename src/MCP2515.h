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
	virtual void ChangeCLKoutPin(const uint_fast8_t OnOffFlag, const uint_fast8_t divider=1);
	virtual const uint_fast8_t Read_Rx_Status() const;
	virtual void SetFilterStandard(const uint_fast8_t Filter, const uint_fast16_t Mask);
	virtual void SetFilterExtended(const uint_fast8_t Filter, const uint_fast32_t Mask);
	//TODO FilterSetzfunktion() einmal für Standard und extended Übergabeparmeter sind (Filter,Maske==Filter)
	//TODO Filteraktivierfunktion(buffer,filter_von_buffer) (evtl als private funktion filtername direkt und andere fkt nur als switch-case)
	//TODO Init-Fkt() allgemein halten und dafür im Konstruktor nach Art der Initalisierung (Interruptpins) unterscheiden
	//TODO Konstruktor aufbohren (Interruptpins)
	//TODO Getter-Methoden für Flags???
	//TODO Sleepfunktion, oder besser erst wenn soweit erweitert das Pin für CAN-Transciver funzt

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
	virtual void SetInterruptPinOnlyForRecive(const uint_fast8_t state=true);
	virtual void SetInterruptPinRegister(const uint_fast8_t Value); //Wert nach CANINTE-Register in Datenblatt
	virtual void Set_RXBx_Interrupt_Pin(const uint_fast8_t Buffer, const uint_fast8_t state=true);
	virtual void Set_Wake_Up_Filer(const uint_fast8_t state); //Schaltet Filter an CAN-Leitungen um versehentliches Aufwachen im sleppmode zu verhindern
	virtual const uint_fast8_t IsBool(const uint_fast8_t& number) const;
	virtual void Change_CLKOUT_PIN_REGISTER(const uint_fast8_t CLKEN_Bit, const uint_fast8_t CLKPRE) const;

	//Attribute:
	uint_fast32_t Bitrate_CAN=0;	//in kB/s
	const float Quartz_Speed=0;		//in MHz

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
};

#endif /* MCP2515_H_ */
