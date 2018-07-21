// Boiler.h

#ifndef _BOILER_h
#define _BOILER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "HeatingControlComponentOut.h"

// This class is a relay that controls the boiler

class Boiler final : public HeatingControlComponentOut { // Do not allow inheritance of Boiler and inherit HeatingControlComponentOut
public: // Public methods and members
	Boiler(int); // Constructor takes an int for the pin number
};

#endif
