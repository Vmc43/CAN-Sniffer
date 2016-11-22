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

#define SPI_SPEED 7800000	//Raspy
//#define SPI_SPEED 1000000	//Bealgeboard
#define Quartz_Speed 8.0
#define Bitrate 1000000

int main()
{
	MCP2515 Test(0,0,Quartz_Speed,Bitrate,SPI_SPEED);
	Test.Read_Rx_Status();

	cout<<"Bitrate: "<<Test.GetBitRate()<<endl;
	return 0;
}
