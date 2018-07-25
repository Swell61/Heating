// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <UIPEthernet.h>
#include <Time.h>

// This class stores the properties of the timer

class Timer {
private:
	
	int midnight = 0; //  Variable to store midnight so the timer can keep track of whether components need to be on or off

	// Store current timer status
	bool heatingTimerState = false;
	bool waterTimerState = false;

	// Store the times for morning and afternoon on and off
	int heatingOnMorning = 420;
	int heatingOffMorning = 540;
	int heatingOnAfternoon = 1200;
	int heatingOffAfternoon = 1320;

	int waterOnMorning = 420;
	int waterOffMorning = 540;
	int waterOnAfternoon = 1200;
	int waterOffAfternoon = 1320;

	int dstOffset(unsigned long time); // Function for calculating the correct daylight savings offset for the time. Takes the current time as a parameter
	unsigned long inline ntpUnixTime(UIPUDP &udp); // Function for getting the time from and NTP server

public:
	// Methods for changing timer states
	void setHeatingTimerState(bool state);
	void setWaterTimerState(bool state);

	// Method for getting timer status
	bool getHeatingTimerStatus();
	bool getWaterTimerStatus();

	int getMidnight(); // Function for getting midnight
	void checkMidnight(UIPUDP &udp); // Function for checking midnight using NTP time
	bool setMidnight(int time); // Function for setting midnight based on parameter
	bool setMidnightNTP(UIPUDP &udp); // Function for setting midnight using NTP time

	// Function for getting timer times
	int getHeatingOnMorning();
	int getHeatingOffMorning();
	int getHeatingOnAfternoon();
	int getHeatingOffAfternoon();
	int getWaterOnMorning();
	int getWaterOffMorning();
	int getWaterOnAfternoon();
	int getWaterOffAfternoon();

	// Function for setting timer times
	bool setHeatingOnMorning(int state);
	bool setHeatingOffMorning(int state);
	bool setHeatingOnAfternoon(int state);
	bool setHeatingOffAfternoon(int state);
	bool setWaterOnMorning(int state);
	bool setWaterOffMorning(int state);
	bool setWaterOnAfternoon(int state);
	bool setWaterOffAfternoon(int state);

	int getTime(); // Function to get current time
	unsigned long getNTPTime(UIPUDP &udp); // Function for getting current time using NTP server
};

#endif

