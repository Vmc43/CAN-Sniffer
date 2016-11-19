/*
 * SPIDevice.cpp  Created on: 22 May 2014
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

#include "SPIDevice.h"
#include <iostream>
#include <iomanip>
#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)  //!< Macro for filling in leading 0 on HEX outputs

/**
 * The constructor for the SPIDevice that sets up and opens the SPI connection.
 * The destructor will close the SPI file connection.
 * @param bus The SPI bus number X (first digit after spidevX.Y)
 * @param device The device on the bus Y (second digit after spidevX.Y)
 */
SPIDevice::SPIDevice(const uint_fast8_t bus, const uint_fast8_t device, uint_fast16_t speed)
	:BusDevice(bus,device)
{
	filename=SPI_PATH+std::to_string(bus)+"."+std::to_string(device);
	mode = SPIDevice::MODE0;
	bits = 8;
	SPIDevice::speed = speed;
	delay = 0;
	open();
}

/**
 * This method opens the file connection to the SPI device.
 * @return 0 on a successful open of the file
 */
const int_fast8_t SPIDevice::open()
{
	const int_fast16_t handler=::open(filename.c_str(), O_RDWR);

	if(handler<0)
	{
		cerr<<"SPI: Can't open device."<<endl;
		return -1;
	}
	else
	{
		SetHandler(handler);
	}

	if(setMode(mode)==-1)
	{
		return -1;
	}

	if(setSpeed(speed)==-1)
	{
		return -1;
	}

	if(setBitsPerWord(bits)==-1)
	{
		return -1;
	}
	active=true;

	return 0;
}

/**
 * Generic method to transfer data to and from the SPI device. It is used by the
 * following methods to read and write registers.
 * @param send The array of data to send to the SPI device
 * @param receive The array of data to receive from the SPI device
 * @param length The length of the array to send
 * @return -1 on failure
 */
const int_fast8_t SPIDevice::transfer(const uint_fast8_t send[], uint_fast8_t receive[], const uint_fast16_t length) const
{
	struct spi_ioc_transfer	transfer;

	//Nullt
	memset(&transfer,0,sizeof(transfer));

	//Werte setzen
	transfer.tx_buf = (__u64) send;
	transfer.rx_buf = (__u64) receive;
	transfer.len = length;
	transfer.speed_hz = speed;
	transfer.bits_per_word = bits;
	transfer.delay_usecs = delay;

	const int_fast8_t status = ioctl(GetHandler(), SPI_IOC_MESSAGE(1), &transfer);
	if(status < 0)
	{
		cerr<<"SPI: SPI_IOC_MESSAGE Failed!"<<endl;
		return -1;
	}

	return status;
}

/**
 *  A simple method to dump the registers to the standard output -- useful for debugging
 *  @param number the number of registers to dump
 */
void SPIDevice::debugDumpRegisters(const uint_fast16_t number) const
{
	cout << "SPI Mode: " << mode << endl;
	cout << "Bits per word: " << (int)bits << endl;
	cout << "Max speed: " << speed << endl;
	cout << "Dumping Registers for Debug Purposes:" << endl;
	const unsigned char *registers = readRegisters(number);
	for(int i=0; i<(int)number; i++)
	{
		cout << HEX(*(registers+i)) << " ";
		if (i%16==15) cout << endl;
	}
	cout << dec;
}

/**
 *   Set the speed of the SPI bus
 *   @param speed the speed in Hz
 */
const int_fast8_t SPIDevice::setSpeed(const uint32_t speed)
{
	const uint_fast8_t Active_vorher=active;
	if(active)
	{
		SPIDevice::close();
	}

	SPIDevice::speed = speed;
	if(ioctl(GetHandler(), SPI_IOC_WR_MAX_SPEED_HZ, &this->speed)==-1)
	{
		cerr<<"SPI: Can't set max speed HZ"<<endl;
		return -1;
	}
	if(ioctl(GetHandler(), SPI_IOC_RD_MAX_SPEED_HZ, &this->speed)==-1)
	{
		cerr<<"SPI: Can't get max speed HZ."<<endl;
		return -1;
	}

	if(Active_vorher)
	{
		SPIDevice::open();
	}

	return 0;
}

/**
 *   Set the mode of the SPI bus
 *   @param mode the enumerated SPI mode
 */
const int_fast8_t SPIDevice::setMode(const SPIDevice::SPIMODE mode)
{
	const uint_fast8_t Active_vorher=active;
	if(active)
	{
		SPIDevice::close();
	}

	SPIDevice::mode = mode;
	if(ioctl(GetHandler(), SPI_IOC_WR_MODE, &mode)==-1)
	{
		cerr<<"SPI: Can't set SPI mode."<<endl;
		return -1;
	}
	if (ioctl(GetHandler(), SPI_IOC_RD_MODE, &mode)==-1)
	{
		cerr<<"SPI: Can't get SPI mode."<<endl;
		return -1;
	}

	if(Active_vorher)
	{
		SPIDevice::open();
	}

	return 0;
}

/**
 *   Set the number of bits per word of the SPI bus
 *   @param bits the number of bits per word
 */
const int_fast8_t SPIDevice::setBitsPerWord(const uint8_t bits)
{
	const uint_fast8_t Active_vorher=active;
	if(active)
	{
		SPIDevice::close();
	}

	SPIDevice::bits = bits;
	if(ioctl(GetHandler(), SPI_IOC_WR_BITS_PER_WORD, &this->bits)==-1)
	{
		cerr<<"SPI: Can't set bits per word."<<endl;
		return -1;
	}
	if(ioctl(GetHandler(), SPI_IOC_RD_BITS_PER_WORD, &this->bits)==-1)
	{
		cerr<<"SPI: Can't get bits per word."<<endl;
		return -1;
	}

	if(Active_vorher)
	{
		SPIDevice::open();
	}

	return 0;
}

/**
 *   Close the SPI device
 */
void SPIDevice::close()
{
	::close(GetHandler());
	SetHandler(-1);
	active=false;
}

/**
 *   The destructor closes the SPI bus device
 */
SPIDevice::~SPIDevice()
{
	close();
}

const int_fast8_t SPIDevice::write(const uint_fast8_t value) const
{
	uint_fast8_t null_return = 0x00;

	return transfer(&value, &null_return, sizeof(value));
}

const int_fast8_t SPIDevice::write(const uint_fast8_t value[], const uint_fast8_t length) const
{
	uint_fast8_t Return[length];
	return transfer(value,Return,length);
}
