// HeatingSystem.h

#ifndef _HEATINGSYSTEM_h
#define _HEATINGSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Pump.h"
#include "Boiler.h"
class HeatingSystem {
private:
	const Pump& pump;
	const Boiler& boiler;
public:
	HeatingSystem(int, int);
	
};

#endif
