/**
 * @file	MCP2515.cpp
 * @date	23.10.2016
 * @author	test
 * @brief	Datei_Beschreibung_Kurz
 *
 * Datei_Beschreibung_Lang
 */

/** Funktion_Beschreibung_Kurz.
 * @param	Parameter_Name Parameter_Beschreibung
 * @return	Rueckgabe_Beschreibung
 *
 * Funktion_Beschreibung_Lang
 */

#include "MCP2515.h"
#include "mcp2515_defs.h"

MCP_2515::MCP_2515(unsigned int bus, unsigned int device, uint_fast16_t speed)
	:SPI(bus, device, speed)
{

}

MCP_2515::~MCP_2515() {
	// TODO Auto-generated destructor stub
}

void MCP_2515::Test()
{
	SPI.readRegister(0x42);
}
