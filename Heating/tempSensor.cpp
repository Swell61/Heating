// 
// 
// 

#include "tempSensor.h"
tempSensor::tempSensor(int pinNum): thermostat(OneWire(pinNum)), temperature(DallasTemperature(&thermostat)) {
};

float tempSensor::getTemp() {
	temperature.requestTemperaturesByAddress(deviceAddress);
	return temperature.getTempC(deviceAddress);
};