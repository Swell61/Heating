// Boiler.h

#ifndef _BOILER_h
#define _BOILER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Boiler {
private:
	int pinNum;
	bool status;
public:
	Boiler(int);
	Boiler();
	void enable();
	void disable();
	bool getStatus();
};