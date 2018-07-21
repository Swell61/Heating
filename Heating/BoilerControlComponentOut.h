// BoilerControlComponentOut.h

#ifndef BoilerControlComponentOut_h
#define BoilerControlComponentOut_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// This is an abstract class for heating components since many components such as the pump and boiler share functions

class BoilerControlComponentOut {
private:
	int pinNum;
	bool status;
	void changeRelayState();
	bool state;
protected:
	BoilerControlComponentOut(int);
public:
	void enable();
	void disable();
	bool getStatus();
	
};

#endif

