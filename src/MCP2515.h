/**
 * @file	MCP2515.h
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

#ifndef MCP2515_H_
#define MCP2515_H_

#include "SPIDevice.h"

using namespace std;

class MCP_2515 {
public:
	MCP_2515(unsigned int bus, unsigned int device, uint_fast16_t speed=500000);
	virtual ~MCP_2515();

	void Test();

private:
	SPIDevice SPI;
};

#endif /* MCP2515_H_ */
