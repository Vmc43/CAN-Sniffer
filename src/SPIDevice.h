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
	SPIDevice(unsigned int bus, unsigned int device, uint_fast16_t speed=500000);
	virtual ~SPIDevice();
	virtual unsigned char readRegister(unsigned int registerAddress)=0;
	virtual unsigned char* readRegisters(unsigned int number, unsigned int fromAddress=0)=0;
	virtual int writeRegister(unsigned int registerAddress, unsigned char value)=0;
	virtual void debugDumpRegisters(unsigned int number = 0xff);
	virtual int setSpeed(uint32_t speed);
	virtual int setMode(SPIDevice::SPIMODE mode);
	virtual int setBitsPerWord(uint8_t bits);
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
	virtual int transfer(unsigned char read[], unsigned char write[], int length);
	virtual int open();
	virtual void close();
};

#endif /* SPIDEVICE_H_ */
