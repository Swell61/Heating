// 
// 
// 

#include "Pump.h"

Pump::Pump(int pinNum) {

}
Pump::Pump() {}

void Pump::enable() {
	status = true;
}

void Pump::disable() {
	status = false;
}

boolean Pump::getStatus() {
	return status;
}