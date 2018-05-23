// HeatingSystem.h

#ifndef _HEATINGSYSTEM_h
#define _HEATINGSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
#include "Pump.h"
#include "Boiler.h"
class HeatingSystem {
private:
	Pump pump;
	Boiler boiler;
public:
	HeatingSystem(int, int);
	void enableHeating();
	void enableHotWater();
};