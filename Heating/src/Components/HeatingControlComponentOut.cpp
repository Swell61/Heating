// 
// 
// 

#include "HeatingControlComponentOut.h"

HeatingControlComponentOut::HeatingControlComponentOut(unsigned char pinNum) : pinNum(pinNum) { // Constructor that takes the pin number
	pinMode(pinNum, OUTPUT); // Set the pin mode to output
	digitalWrite(pinNum, LOW); // Set the output to low (disables the relay and therefore the component(
}

void HeatingControlComponentOut::enable() { // Function that will turn on the specific heating system component
	state = State::ON; // Set the status to true
	changeRelayState(); // Change the relay state
}

void HeatingControlComponentOut::disable() { // Function that will turn off the specific heating system component
	state = State::OFF; // Set the status to false
	changeRelayState(); // Change the relay state
}

State HeatingControlComponentOut::getState() { // Function that returns the status of the component
	return state; // Return the status
}

void HeatingControlComponentOut::changeRelayState() { // Function that changed the realay state
	digitalWrite(pinNum, state == State::ON); // Change the output to the required status
}