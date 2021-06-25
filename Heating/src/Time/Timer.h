// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#include "Arduino.h"
#include <UIPEthernet.h>
#include <DS3232RTC.h> 
#include "../Enums/SystemFunction.h"
#include "../Enums/TimeComponent.h"
#include "../Enums/ValueAdjustment.h"
#include "../Enums/TimerPeriod.h"
#include "../Enums/SystemComponent.h"
#include "../Enums/State.h"
#include "ComponentTimer.h"
// This class stores the properties of the timer

class Timer {
private:
	int midnight = 0; //  Variable to store midnight so the timer can keep track of whether components need to be on or off

	// Store current timer status
	bool heatingTimerState = false;
	bool waterTimerState = false;

	// Store the times for morning and afternoon on and off
	ComponentTimer heating;
	ComponentTimer water;

	bool adjustTime(bool (Timer::*updateFunction)(unsigned short int), int currentValue, ValueAdjustment adjustmentDirection, int adjustmentValue);
	int dstOffset(unsigned long time); // Function for calculating the correct daylight savings offset for the time. Takes the current time as a parameter
	unsigned long inline ntpUnixTime(UIPUDP &udp); // Function for getting the time from and NTP server

public:
	Timer();
	// Methods for changing timer states
	void setHeatingTimerState(bool state);
	void setWaterTimerState(bool state);

	const ComponentTimer& getHeatingTimer() const;
	const ComponentTimer& getWaterTimer() const;
	bool getHeatingTimerStatus(); // Should be const but Time library is not const correct
	bool getWaterTimerStatus(); // Should be const but Time library is not const correct

	bool setSystemTime(unsigned long time);
	bool setSystemHour(unsigned short int);
	bool setSystemMinute(unsigned short int newMinute);
	int getHour(); 
	int getMinute();
	int getTimeInMinutes(); // Function to get current time
	bool adjustTime(TimeComponent timeComponent, ValueAdjustment adjustment);
	bool adjustTimer(SystemComponent component, TimerPeriod period, State state,  ValueAdjustment adjustment);
	unsigned long getNTPTime(UIPUDP &udp); // Function for getting current time using NTP server
};

#endif

