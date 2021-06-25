// HeatingControlComponentOut.h

#ifndef HeatingControlComponentOut_h
#define HeatingControlComponentOut_h

#include "Arduino.h"

// This is an abstract class for heating components since many components such as the pump and boiler share functions

class HeatingControlComponentOut {
private:
	int pinNum;
	bool status;
	void changeRelayState();
	bool state;
protected:
	HeatingControlComponentOut(int pinNum);
public:
	void enable();
	void disable();
	bool getStatus();
	
};

#endif

