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
SPIDevice::SPIDevice(unsigned int bus, unsigned int device, uint_fast16_t speed)
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
const int SPIDevice::open()
{
	const int handler=::open(filename.c_str(), O_RDWR);

	if(handler)
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
const int SPIDevice::transfer(unsigned char send[], unsigned char receive[], int length)
{
	struct spi_ioc_transfer	transfer;
	memset(&transfer,0,sizeof(transfer));
	transfer.tx_buf = (unsigned long) send;
	transfer.rx_buf = (unsigned long) receive;
	transfer.len = length;
	transfer.speed_hz = this->speed;
	transfer.bits_per_word = this->bits;
	transfer.delay_usecs = this->delay;
	int status = ioctl(GetHandler(), SPI_IOC_MESSAGE(1), &transfer);
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
void SPIDevice::debugDumpRegisters(const unsigned int number)
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
const int SPIDevice::setSpeed(uint32_t speed)
{
	uint_fast8_t Active_vorher=active;
	if(active)
	{
		SPIDevice::close();
	}

	SPIDevice::speed = speed;
	if (ioctl(GetHandler(), SPI_IOC_WR_MAX_SPEED_HZ, &this->speed)==-1)
	{
		perror("SPI: Can't set max speed HZ");
		return -1;
	}
	if (ioctl(GetHandler(), SPI_IOC_RD_MAX_SPEED_HZ, &this->speed)==-1)
	{
		perror("SPI: Can't get max speed HZ.");
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
const int SPIDevice::setMode(SPIDevice::SPIMODE mode)
{
	uint_fast8_t Active_vorher=active;
	if(active)
	{
		SPIDevice::close();
	}

	SPIDevice::mode = mode;
	if(ioctl(GetHandler(), SPI_IOC_WR_MODE, &mode)==-1)
	{
		perror("SPI: Can't set SPI mode.");
		return -1;
	}
	if (ioctl(GetHandler(), SPI_IOC_RD_MODE, &mode)==-1)
	{
		perror("SPI: Can't get SPI mode.");
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
const int SPIDevice::setBitsPerWord(uint8_t bits)
{
	uint_fast8_t Active_vorher=active;
	if(active)
	{
		SPIDevice::close();
	}

	SPIDevice::bits = bits;
	if(ioctl(GetHandler(), SPI_IOC_WR_BITS_PER_WORD, &this->bits)==-1)
	{
		perror("SPI: Can't set bits per word.");
		return -1;
	}
	if(ioctl(GetHandler(), SPI_IOC_RD_BITS_PER_WORD, &this->bits)==-1)
	{
		perror("SPI: Can't get bits per word.");
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

const int SPIDevice::write(const unsigned char value)
{

}
