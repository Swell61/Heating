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
	TempSensor(unsigned char pinNum, const unsigned char addrB1, const unsigned char addrB2, const unsigned char addrB3, const unsigned char addrB4,
	 const unsigned char addrB5, const unsigned char addrB6, const unsigned char addrB7, const unsigned char addrB8); // Constructor that takes the pin number of the temperature sensor
private:
	//LM75 sensor; // Temperature sensor
	float lastTemp;
	unsigned long lastTempUpdate;
	OneWire oneWire;
	DallasTemperature sensor;
	const unsigned char ADDRESS[8];
	unsigned long lastRequest = 0;
};

#endif

