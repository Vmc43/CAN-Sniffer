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
	virtual const uint_fast8_t readRegister(const uint_fast16_t registerAddress)const=0;
	virtual const uint_fast8_t* readRegisters(const uint_fast8_t number, const uint_fast16_t fromAddress=0)const=0;
	virtual const int_fast8_t write(const uint_fast8_t value)const=0; //Only for debug
	virtual const int_fast8_t writeRegister(const uint_fast16_t registerAddress, const uint_fast8_t value)const=0;
	virtual void close()=0;
	virtual ~BusDevice();
	virtual inline const uint_fast8_t GetBus()const{return bus;}
	virtual inline const uint_fast8_t GetDevice()const{return device;}
	virtual inline const int_fast16_t GetHandler()const{return handler;}
	virtual inline void SetHandler(const int_fast16_t handler){BusDevice::handler=handler;}
private:
	const uint_fast8_t bus;    /**< the bus number */
	const uint_fast8_t device; /**< the device number on the bus  */
	int handler;            /**< the file handle to the device */
};

#endif /* BUSDEVICE_H_ */
