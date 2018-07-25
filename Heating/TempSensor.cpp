// 
// 
// 

#include "TempSensor.h"
TempSensor::TempSensor(int pinNum) : oneWire(OneWire(pinNum)), sensor(DallasTemperature(&oneWire)) { // Constructor that sets up the temperature sensor
	sensor.begin();
	sensor.setWaitForConversion(false);
	sensor.setResolution(address, 10);
	sensor.requestTemperaturesByAddress(address);
	lastRequest = millis();
};

float TempSensor::getTemp(){ // Returns the current temperature from the sensor
	if ((millis() - lastRequest) >= 190) { // If the last request has been fulfilled
		lastTemp = sensor.getTempC(address); // New temperature will be ready
		sensor.requestTemperaturesByAddress(address); // Request again
		lastRequest = millis();
	}
	// If new temp is not yet ready, return the previous temp
	return lastTemp;
};
