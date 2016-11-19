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

int main()
{
	MCP2515 Test(0,0,SPI_SPEED);

	cout << "!!!Hello World!!!" << endl;
	return 0;
}
