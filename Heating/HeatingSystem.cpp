// 
// 
// 

#include "HeatingSystem.h"

HeatingSystem::HeatingSystem(int pumpPin, int boilerPin) {
	pump = Pump(pumpPin);
	boiler = Boiler(boilerPin);
}


