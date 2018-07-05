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
#include "Timer.h"
class HeatingSystem {
private:
	const byte boostLengthWater = 30;
	const byte boostLengthHeating = 1;
	int boostTimerHeating = 0;
	int boostTimerWater = 0;
	const int maxDrift = 1; // Set the maximum number of degrees the temperature can drift before heating is turned on
	bool updateDisplay = false;
	byte screen = 0; // 0 = Regular screen. 1 = Timer screen

	int requestedTemp = 15;
	bool heatingStatus = false;
	bool waterStatus = false;
	bool heatingMaster = true;
	bool heatingBoostActive = false;
	bool waterBoostActive = false;
	bool incorrectTemp = false;
	unsigned long startTimeHeatingBoost;
	unsigned long startTimeWaterBoost;
	byte touchOption;
	float currentTemp;

	Pump pump;
	Boiler boiler;
	TempSensor tempSensor;
	Display* display;
	Timer timer;
	void setHeatingOff();
	void setWaterOff();
	void setHeatingOn();
	void setWaterOn();
	void setWaterWithoutHeating();
	void checkBoosts();
	void temperatureCheck();
	void changeRelayStates();
	void checkTimer();
	
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
