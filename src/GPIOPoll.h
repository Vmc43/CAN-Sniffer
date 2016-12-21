/*
 * GPIOPoll.h
 *
 *  Created on: 09.12.2016
 *      Author: test
 */

#ifndef GPIOPOLL_H_
#define GPIOPOLL_H_

#include <thread>
#include <cstdint>
#include <string>

using namespace std;

class MCP2515;

class GPIO_Poll
{
public:
	GPIO_Poll(const MCP2515* Mamaklasse);
	GPIO_Poll(const MCP2515* Mamaklasse, const uint_fast8_t PinNummer);
	virtual ~GPIO_Poll();

	virtual void Start();
	inline virtual void SetPinNummer(const uint_fast8_t PinNummer){GPIO_Poll::PinNummer=PinNummer;}

private:
	virtual int_fast8_t WaitForEdge(); // threaded with callback
	virtual void TreadMain();

	std::thread The_Thread;
	uint_fast8_t Stop_Thread = false; // Super simple thread stopping.
	const MCP2515* Mamaklasse=NULL;
	uint_fast8_t PinNummer=0;
	const string Pfad;
};

#endif /* GPIOPOLL_H_ */
