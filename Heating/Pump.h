// Pump.h

#ifndef _PUMP_h
#define _PUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "BoilerControlComponentOut.h"

// This class is a pump

class Pump final : public BoilerControlComponentOut { // Do not allow inheritance of Pump and inherit BoilerControlComponentOut
public:
	Pump(int); // Constructor that takes the pin number for the pump relay
};

#endif
