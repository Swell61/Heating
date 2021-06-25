// tempSensor.h

#ifndef _TEMPSENSOR_h
#define _TEMPSENSOR_h

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <LM75.h>

// This class is a temperature sensor

class TempSensor {
public:
	float getTemp(); // Function for getting the current temperature from the sensor
	TempSensor(); // Constructor that takes the pin number of the temperature sensor
	TempSensor(uint8_t newAddress[8]);
	static void begin(int pinNum);
private:
	//LM75 sensor; // Temperature sensor
	float lastTemp;
	unsigned long lastTempUpdate;
	static OneWire oneWire;
	static DallasTemperature sensor;
	uint8_t address[8] = { 0x28, 0xFF, 0x60, 0x9A, 0xA1, 0x17, 0x5, 0x43 };
	unsigned long lastRequest = 0;
	void initialiseTempSensor();
};

#endif

