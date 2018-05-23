// 
// 
// 

#include "BoilerControlComponentOut.h"

BoilerControlComponentOut::BoilerControlComponentOut(int pinNum) : pinNum(pinNum) {
	pinMode(pinNum, OUTPUT);
}

void BoilerControlComponentOut::enable() {
	status = true;
	changeRelayState();
}

void BoilerControlComponentOut::disable() {
	status = false;
	changeRelayState();
}

boolean BoilerControlComponentOut::getStatus() {
	return status;
}

void BoilerControlComponentOut::changeRelayState() {
	digitalWrite(pinNum, status);
}