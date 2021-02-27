// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <UIPEthernet.h>
#include <DS3232RTC.h> 
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
	Timer();
	// Methods for changing timer states
	void setHeatingTimerState(bool state);
	void setWaterTimerState(bool state);

	// Method for getting timer status
	bool getHeatingTimerStatus();
	bool getWaterTimerStatus();

	bool setSystemTime(unsigned long time);
	bool setSystemHour(int newHour);
	bool setSystemMinute(int newMinute);
	int getHour(); 
	int getMinute();
	int getTimeInMinutes(); // Function to get current time
	unsigned long getNTPTime(UIPUDP &udp); // Function for getting current time using NTP server

	// Function for getting timer times
	int getHeatingOnMorning() const;
	int getHeatingOffMorning() const;
	int getHeatingOnAfternoon() const;
	int getHeatingOffAfternoon() const;
	int getWaterOnMorning() const;
	int getWaterOffMorning() const;
	int getWaterOnAfternoon() const;
	int getWaterOffAfternoon() const;

	// Function for setting timer times
	bool setHeatingOnMorning(int state);
	bool setHeatingOffMorning(int state);
	bool setHeatingOnAfternoon(int state);
	bool setHeatingOffAfternoon(int state);
	bool setWaterOnMorning(int state);
	bool setWaterOffMorning(int state);
	bool setWaterOnAfternoon(int state);
	bool setWaterOffAfternoon(int state);

	
};

#endif

