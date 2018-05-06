// Pump.h

#ifndef _PUMP_h
#define _PUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Pump
{
private:
	int pinNum;
	bool status;
public:
	Pump(int);
	Pump();
	void enable();
	void disable();
	bool getStatus();
};