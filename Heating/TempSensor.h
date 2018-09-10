// tempSensor.h

#ifndef _TEMPSENSOR_h
#define _TEMPSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <OneWire.h>
#include <DallasTemperature.h>
//#include <LM75.h>

// This class is a temperature sensor

class TempSensor {
public:
	float getTemp(); // Function for getting the current temperature from the sensor
	TempSensor(int pinNum); // Constructor that takes the pin number of the temperature sensor
	TempSensor(int pinNum, uint8_t newAddress[8]);
private:
	//LM75 sensor; // Temperature sensor
	float lastTemp;
	unsigned long lastTempUpdate;
	OneWire oneWire;
	DallasTemperature sensor;
	uint8_t address[8] = { 0x28, 0xFF, 0x60, 0x9A, 0xA1, 0x17, 0x5, 0x43 };
	unsigned long lastRequest = 0;
	void initialiseTempSensor();
};

#endif

