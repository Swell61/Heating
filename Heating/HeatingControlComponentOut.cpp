// 
// 
// 

#include "HeatingControlComponentOut.h"

HeatingControlComponentOut::HeatingControlComponentOut(int pinNum) : pinNum(pinNum) { // Constructor that takes the pin number
	pinMode(pinNum, OUTPUT); // Set the pin mode to output
	digitalWrite(pinNum, LOW); // Set the output to low (disables the relay and therefore the component(
}

void HeatingControlComponentOut::enable() { // Function that will turn on the specific heating system component
	status = true; // Set the status to true
	changeRelayState(); // Change the relay state

}

void HeatingControlComponentOut::disable() { // Function that will turn off the specific heating system component
	status = false; // Set the status to false
	changeRelayState(); // Change the relay state
}

boolean HeatingControlComponentOut::getStatus() { // Function that returns the status of the component
	return status; // Return the status
}

void HeatingControlComponentOut::changeRelayState() { // Function that changed the realay state
	digitalWrite(pinNum, status); // Change the output to the required status
}