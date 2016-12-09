//============================================================================
// Name        : CAN-Sniffer.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "MCP2515.h"

using namespace std;

#define SPI_SPEED 500000	//Raspy
//#define SPI_SPEED 1000000	//Bealgeboard
#define Quartz_Speed 8000000
#define Bitrate 125000

int main()
{
	MCP2515 Test((uint_fast8_t)0,(uint_fast8_t)0,(uint_fast32_t)Quartz_Speed,(uint_fast32_t)Bitrate,(uint_fast8_t)9,(uint_fast32_t)SPI_SPEED);
	//Test.Read_Rx_Status();

	//cout<<"Bitrate: "<<Test.GetBitRate()<<endl;
	return 0;
}
