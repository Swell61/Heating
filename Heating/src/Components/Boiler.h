// Boiler.h

#ifndef _BOILER_h
#define _BOILER_h

#include "Arduino.h"
#include "HeatingControlComponentOut.h"

// This class is a relay that controls the boiler

class Boiler final : public HeatingControlComponentOut { // Do not allow inheritance of Boiler and inherit HeatingControlComponentOut
public: // Public methods and members
	Boiler(unsigned char pinNum); // Constructor takes an int for the pin number
};

#endif
