// 
// 
// 

#include "TempSensor.h"
TempSensor::TempSensor(int pinNum): thermostat(OneWire(pinNum)), temperature(DallasTemperature(&thermostat)) {
};

float TempSensor::getTemp() const{
	temperature.requestTemperaturesByAddress(deviceAddress);
	return temperature.getTempC(deviceAddress);
};