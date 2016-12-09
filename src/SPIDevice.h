#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_
#include <string>
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
	SPIDevice(const uint_fast8_t bus, const uint_fast8_t device, const uint_fast32_t speed=500000);
	virtual ~SPIDevice();
	virtual const uint_fast8_t readRegister(const uint_fast16_t registerAddress)const=0;
	virtual const uint_fast8_t* readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress=0)const=0;
	virtual const int_fast8_t writeRegister(const uint_fast16_t registerAddress, const uint_fast8_t value)const=0;
	virtual const int_fast8_t write(const uint_fast8_t value) const;
	virtual const int_fast8_t write(const uint_fast8_t value[], const uint_fast8_t length) const;
	virtual void debugDumpRegisters(const uint_fast16_t number = 0xff) const; //Registerwert ausgeben
	virtual const int_fast8_t setSpeed(const uint32_t speed);
	virtual const int_fast8_t setMode(const SPIDevice::SPIMODE mode);
	virtual const int_fast8_t setBitsPerWord(const uint8_t bits);
	virtual const int_fast8_t transfer(const uint_fast8_t send[], uint_fast8_t receive[], const uint_fast16_t length) const;
	virtual inline const uint_fast8_t GetBitsPerWord() const {return bits;}
	virtual inline const uint_fast32_t GetSpeed() const {return speed;}
	virtual inline const uint_fast8_t GetDelay() const {return delay;}
	virtual inline const uint_fast8_t GetActiveFlag() const {return active;}

private:
	SPIMODE mode;     //!< The SPI mode as per the SPIMODE enumeration
	uint_fast8_t bits;     //!< The number of bits per word
	uint_fast32_t speed;   //!< The speed of transfer in Hz
	uint_fast16_t delay;   //!< The transfer delay in usecs
	string filename;  //!< The precise filename for the SPI device
	uint_fast8_t active=false;
	virtual const int_fast8_t open();
	virtual void close();
};

#endif /* SPIDEVICE_H_ */
