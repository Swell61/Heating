// Pump.h

#ifndef _PUMP_h
#define _PUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "BoilerControlComponentOut.h"
class Pump final : public BoilerControlComponentOut {
public:
	Pump(int);
};

#endif