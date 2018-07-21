// tempSensor.h

#ifndef _TEMPSENSOR_h
#define _TEMPSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include <LM75.h>

// This class is a temperature sensor

class TempSensor {
public:
	float getTemp() const; // Function for getting the current temperature from the sensor
	TempSensor(int pinNum); // Constructor that takes the pin number of the temperature sensor
private:
	LM75 sensor; // Temperature sensor

};

#endif

