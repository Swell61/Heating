// 
// 
// 

#include "TempSensor.h"
TempSensor::TempSensor(unsigned char pinNum, const unsigned char addrB1, const unsigned char addrB2, const unsigned char addrB3, const unsigned char addrB4,
	 const unsigned char addrB5, const unsigned char addrB6, const unsigned char addrB7, const unsigned char addrB8) : oneWire(OneWire(pinNum)), 
	 ADDRESS{addrB1, addrB2, addrB3, addrB4, addrB5, addrB6, addrB7, addrB8} { // Constructor that sets up the temperature sensor
	sensor = DallasTemperature(&oneWire);
	sensor.begin();
	sensor.setWaitForConversion(false);
	sensor.setResolution(ADDRESS, 10);
	sensor.requestTemperaturesByAddress(ADDRESS);
	lastRequest = millis();
};

float TempSensor::getTemp(){ // Returns the current temperature from the sensor
	if ((millis() - lastRequest) >= 190) { // If the last request has been fulfilled
		lastTemp = sensor.getTempC(ADDRESS); // New temperature will be ready
		sensor.requestTemperaturesByAddress(ADDRESS); // Request again
		lastRequest = millis();
	}
	// If new temp is not yet ready, return the previous temp
	if (lastTemp < 0) {
		lastTemp = 99; // If temperature sensor becomes fautly, stop thermostat from keeping heatng on
	}
	return lastTemp;
};
