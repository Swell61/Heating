// 
// 
// 

#include "TempSensor.h"
OneWire TempSensor::oneWire = NULL;
DallasTemperature TempSensor::sensor;
TempSensor::TempSensor() { // Constructor that sets up the temperature sensor
	initialiseTempSensor();
};
TempSensor::TempSensor(uint8_t newAddress[8]) {
	for (byte i = 0; i <= 7; i++) {
		address[i] = newAddress[i];
	}
	initialiseTempSensor();
}
void TempSensor::begin(int pinNum) {
	oneWire = OneWire(pinNum);
	sensor = DallasTemperature(&oneWire);
	sensor.begin();
	sensor.setWaitForConversion(false);

}
void TempSensor::initialiseTempSensor() {
	sensor.setResolution(address, 10);
	sensor.requestTemperaturesByAddress(address);
	lastRequest = millis();
}
float TempSensor::getTemp(){ // Returns the current temperature from the sensor
	if ((millis() - lastRequest) >= 190) { // If the last request has been fulfilled
		lastTemp = sensor.getTempC(address); // New temperature will be ready
		sensor.requestTemperaturesByAddress(address); // Request again
		lastRequest = millis();
	}
	// If new temp is not yet ready, return the previous temp
	if (lastTemp < 0) {
		lastTemp = 99; // If temperature sensor becomes fautly, stop thermostat from keeping heatng on
	}
	return lastTemp;
};
