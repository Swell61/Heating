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
private:
	//LM75 sensor; // Temperature sensor
	float lastTemp;
	unsigned long lastTempUpdate;
	OneWire oneWire;
	DallasTemperature sensor;
	uint8_t address[8] = { 0x28, 0xFF, 0x9D, 0x10, 0xB0, 0x16, 0x04, 0xB0 };
	unsigned long lastRequest = 0;
	
};

#endif

