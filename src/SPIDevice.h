/*
 * SPIDevice.h  Created on: 22 May 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_
#include <string>
#include <stdint.h>
#include "BusDevice.h"

#define SPI_PATH "/dev/spidev" /**< The general path to an SPI device **/

using namespace std;

/**
 * @class SPIDevice
 * @brief Generic SPI Device class that can be used to connect to any type of SPI device and read or write to its registers
 */
class SPIDevice:public BusDevice
{
public:
	/// The SPI Mode
	enum SPIMODE{
		MODE0 = 0,   //!< Low at idle, capture on rising clock edge
		MODE1 = 1,   //!< Low at idle, capture on falling clock edge
		MODE2 = 2,   //!< High at idle, capture on falling clock edge
		MODE3 = 3    //!< High at idle, capture on rising clock edge
	};

public:
	SPIDevice(const uint_fast8_t bus, const uint_fast8_t device, uint_fast16_t speed=500000);
	virtual ~SPIDevice();
	virtual const uint_fast8_t readRegister(const uint_fast16_t registerAddress)=0;
	virtual const uint_fast8_t* readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress=0)=0;
	virtual const int_fast8_t writeRegister(const uint_fast16_t registerAddress, const uint_fast8_t value)=0;
	virtual const int_fast8_t write(const uint_fast8_t value); //Only for debug
	virtual void debugDumpRegisters(const uint_fast16_t number = 0xff); //Registerwert ausgeben
	virtual const int_fast8_t setSpeed(const uint32_t speed);
	virtual const int_fast8_t setMode(const SPIDevice::SPIMODE mode);
	virtual const int_fast8_t setBitsPerWord(const uint8_t bits);
	virtual inline const uint_fast8_t GetBitsPerWord(){return bits;}
	virtual inline const uint_fast8_t GetSpeed(){return speed;}
	virtual inline const uint_fast8_t GetDelay(){return delay;}
	virtual inline const uint_fast8_t GetActiveFlag(){return active;}

private:
	SPIMODE mode;     //!< The SPI mode as per the SPIMODE enumeration
	uint_fast8_t bits;     //!< The number of bits per word
	uint_fast32_t speed;   //!< The speed of transfer in Hz
	uint_fast16_t delay;   //!< The transfer delay in usecs
	string filename;  //!< The precise filename for the SPI device
	uint_fast8_t active=false;
	virtual const int_fast8_t transfer(const uint_fast8_t send[], uint_fast8_t receive[], const uint_fast32_t length);
	virtual const int_fast8_t open();
	virtual void close();
};

#endif /* SPIDEVICE_H_ */
