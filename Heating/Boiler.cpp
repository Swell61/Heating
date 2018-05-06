// 
// 
// 

#include "Boiler.h"
Boiler::Boiler(int pinNum) {

}
Boiler::Boiler() {}

void Boiler::enable() {
	status = true;
}

void Boiler::disable() {
	status = false;
}

boolean Boiler::getStatus() {
	return status;
}

