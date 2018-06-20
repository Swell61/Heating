// BoilerControlComponentOut.h

#ifndef BoilerControlComponentOut_h
#define BoilerControlComponentOut_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
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

