/*
 * BusDevice.h  Created on: 23 May 2014
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

#ifndef BUSDEVICE_H_
#define BUSDEVICE_H_

#include <cstdint>

using namespace std;

/**
 * @class BusDevice
 * @brief This class is the parent of I2C and SPI devices, so that devices that use both
 * SPI and I2C interfaces can use those interfaces interchangeably. Because it contains
 * abstract methods, the child classes MUST implement the methods that are listed in this
 * class.
 */
class BusDevice
{
public:
	BusDevice(const uint_fast8_t bus, const uint_fast8_t device);
	virtual const int_fast8_t open()=0;
	virtual const uint_fast8_t readRegister(const uint_fast16_t registerAddress)=0;
	virtual const uint_fast8_t* readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress=0)=0;
	virtual const int_fast8_t write(const uint_fast8_t value)=0; //Only for debug
	virtual const int_fast8_t writeRegister(const uint_fast16_t registerAddress, const uint_fast8_t value)=0;
	virtual void close()=0;
	virtual ~BusDevice();
	virtual inline const uint_fast8_t GetBus(){return bus;}
	virtual inline const uint_fast8_t GetDevice(){return device;}
	virtual inline const int_fast16_t GetHandler(){return handler;}
	virtual inline void SetHandler(const int_fast16_t handler){BusDevice::handler=handler;}
private:
	const uint_fast8_t bus;    /**< the bus number */
	const uint_fast8_t device; /**< the device number on the bus  */
	int handler;            /**< the file handle to the device */
};

#endif /* BUSDEVICE_H_ */
