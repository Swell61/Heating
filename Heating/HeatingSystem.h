// HeatingSystem.h

#ifndef _HEATINGSYSTEM_h
#define _HEATINGSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Pump.h"
#include "Boiler.h"
#include "TempSensor.h"
#include "Display.h"
class HeatingSystem {
private:
	const byte boostLengthWater = 30;
	const byte boostLengthHeating = 30;
	int boostTimerHeating = 0;
	int boostTimerWater = 0;
	const int maxDrift = 2; // Set the maximum number of degrees the temperature can drift before heating is turned on
	
	int requestedTemp = 15;
	bool heatingStatus;
	bool waterStatus;
	bool heatingMaster;
	bool heatingBoostActive;
	bool waterBoostActive;
	unsigned long startTimeHeatingBoost;
	unsigned long startTimeWaterBoost;
	byte touchOption;

	Pump pump;
	Boiler boiler;
	TempSensor tempSensor;
	Display* display;
	void setHeatingOff();
	void setWaterOff();
	void setHeatingOn();
	void setWaterOn();
	void setWaterWithoutHeating();
	void checkBoosts();
	
public:
	HeatingSystem(int, int, int);
	void monitorSystem();
	void boostHeating(bool);
	void boostWater(bool);
	bool getHeatingStatus();
	bool getWaterStatus();
	void disableHeating();
	void disableWater();
	void enableHeating();
	void enableWater();
	void setTemp(int);

};

#endif
