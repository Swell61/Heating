// Boiler.h

#ifndef _BOILER_h
#define _BOILER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class Boiler : public BoilerControlComponentOut {
public:
	Boiler(int);
};