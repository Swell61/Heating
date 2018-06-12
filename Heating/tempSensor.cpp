// 
// 
// 

#include "TempSensor.h"
TempSensor::TempSensor(int pinNum) {
	Wire.begin();
	sensor.shutdown(true);
};

float TempSensor::getTemp() const{
	sensor.shutdown(false);
	float temp = sensor.temp();
	sensor.shutdown(true);
	return temp;
};