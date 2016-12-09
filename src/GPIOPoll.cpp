/*
 * GPIOPoll.cpp
 *
 *  Created on: 09.12.2016
 *      Author: test
 */

#include "GPIOPoll.h"
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>

GPIO_Poll::GPIO_Poll(const MCP2515* Mamaklasse)
	:Mamaklasse(Mamaklasse)
{
	// TODO Auto-generated constructor stub
}

GPIO_Poll::GPIO_Poll(const MCP2515* Mamaklasse, const uint_fast8_t PinNummer)
	:Mamaklasse(Mamaklasse),
	 PinNummer(PinNummer)
{

}

GPIO_Poll::~GPIO_Poll()
{
	Stop_Thread = true;
	if(The_Thread.joinable())
	{
		The_Thread.join();
	}
}

void GPIO_Poll::Start()
{
	The_Thread = std::thread(&GPIO_Poll::WaitForEdge,this);
}

void GPIO_Poll::ThreadMain()
{

}

int_fast8_t GPIO_Poll::WaitForEdge()
{
	//http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_GPIO_C.html
}
