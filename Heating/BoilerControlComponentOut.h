// BoilerControlComponentOut.h

#ifndef _BoilerControlComponentOut_h
#define _BoilerControlComponentOut_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

class BoilerControlComponentOut {
private:
	const int pinNum;
	bool status;
	void changeRelayState();
protected:
	BoilerControlComponentOut(int);
public:
	void enable();
	void disable();
	bool getStatus();
};