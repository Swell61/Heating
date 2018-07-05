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
	
	int midnight;

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

	void setTime(int);
public:
	void heatingMorningOn();
	void heatingMorningOff();
	void heatingAfternoonOn();
	void heatingAfternoonOff();

	void waterMorningOn();
	void waterMorningOff();
	void waterAfternoonOn();
	void waterAfternoonOff();

	bool getHeatingTimerStatus();
	bool getWaterTimerStatus();
	void checkMidnight();
};

#endif

