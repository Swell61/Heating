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
class TempSensor {
public:
	float getTemp() const;
	TempSensor(int pinNum);
private:
	LM75 sensor;
};

#endif

