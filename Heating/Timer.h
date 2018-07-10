// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Timer {
private:
	
	int midnight = 0;

	bool heatingTimerState = false;
	bool waterTimerState = false;

	int heatingOnMorning = 420;
	int heatingOffMorning = 540;
	int heatingOnAfternoon = 1200;
	int heatingOffAfternoon = 1320;

	int waterOnMorning = 420;
	int waterOffMorning = 540;
	int waterOnAfternoon = 1200;
	int waterOffAfternoon = 1320;

	

public:
	void setHeatingTimerState(bool);
	void setWaterTimerState(bool);

	bool getHeatingTimerStatus();
	bool getWaterTimerStatus();
	int getMidnight();
	void checkMidnight();
	bool setMidnight(int);
	int getHeatingOnMorning();
	int getHeatingOffMorning();
	int getHeatingOnAfternoon();
	int getHeatingOffAfternoon();
	int getWaterOnMorning();
	int getWaterOffMorning();
	int getWaterOnAfternoon();
	int getWaterOffAfternoon();

	bool setHeatingOnMorning(int);
	bool setHeatingOffMorning(int);
	bool setHeatingOnAfternoon(int);
	bool setHeatingOffAfternoon(int);
	bool setWaterOnMorning(int);
	bool setWaterOffMorning(int);
	bool setWaterOnAfternoon(int);
	bool setWaterOffAfternoon(int);

	int getTime();
};

#endif

