// 
// 
// 

#include "TempSensor.h"
TempSensor::TempSensor(int pinNum) { // Constructor that sets up the temperature sensor
	Wire.begin();
	sensor.shutdown(true);
};

float TempSensor::getTemp() const{ // Returns the current temperature from the sensor
	sensor.shutdown(false);
	float temp = sensor.temp();
	sensor.shutdown(true);
	return temp;

};