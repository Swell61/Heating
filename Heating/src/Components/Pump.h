// Pump.h

#ifndef _PUMP_h
#define _PUMP_h

#include "Arduino.h"

#include "HeatingControlComponentOut.h"

// This class is a pump

class Pump final : public HeatingControlComponentOut { // Do not allow inheritance of Pump and inherit HeatingControlComponentOut
public:
	Pump(unsigned char pinNum); // Constructor that takes the pin number for the pump relay
};

#endif
