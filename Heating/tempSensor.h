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
class TempSensor {
public:
	float getTemp() const;
	TempSensor(int pinNum);
private:
	const uint8_t deviceAddress[8] = { 0x28 };
	const OneWire& thermostat;
	const DallasTemperature& temperature;
};

#endif

