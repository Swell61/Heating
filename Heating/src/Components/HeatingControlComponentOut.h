// HeatingControlComponentOut.h

#ifndef HeatingControlComponentOut_h
#define HeatingControlComponentOut_h

#include "Arduino.h"
#include "../Enums/State.h"
// This is an abstract class for heating components since many components such as the pump and boiler share functions

class HeatingControlComponentOut {
private:
	unsigned char pinNum;
	bool state;
	void changeRelayState();
protected:
	HeatingControlComponentOut(unsigned char pinNum);
public:
	void enable();
	void disable();
	bool getState();
	
};

#endif

