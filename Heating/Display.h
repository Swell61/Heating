// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <TouchScreen.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

// This class controls the touch screen display on the thermostat. It contains the TFT components and the touch components

class Display {
private:
	MCUFRIEND_kbv tft; // The LCD screen
	TouchScreen ts; // The touschreen
	uint16_t xpos, ypos;  // Screen touch coordinates
	
public:
	Display(); // Default constructor
	void mainDisplay(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost); // Function that displays the main properties of the system on the touchscreen. Parameters taken are: current time, heating mode, hot water mode, current temperature, heating status, hot water status, requested temperature, heating boost status and hot water boost status
	byte touchUpdate(byte screen); // Function that checks the touchscreen for any presses. Returns the required operation as a byte and takes the current screen number as a parameter (The screen number determines the required button coordinates that need to be checked)
	void displayUpdate(int time, byte heatingMode, byte waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost); // Function that updates the main display. Parameters taken are same as mainDisplay
	void timerDisplay(bool heatingTimerStatus, bool waterTimerStatus, int heatingOnMorning, int heatingOffMorning, int heatingOnAfternoon, int heatingOffAfternoon, int waterOnMorning, int waterOffMorning, int waterOnAfternoon, int waterOffAfternoon); // Function that displays the timer. Parameters taken are: heating timer status, hot water timer status, time that heating turns on in the morning, time that heating turns off in the morning, time that the heating turns on in the afternoon, time that the heating turns off in the afternoon, time that the hot water turns on in the morning, time that the hot water turns off in the morning, time that the hot water turns on in the afternoon and the time that the hot water turns off in the afternoon
	void timerUpdate(bool heatingTimerStatus, bool waterTimerStatus, int heatingOnMorning, int heatingOffMorning, int heatingOnAfternoon, int heatingOffAfternoon, int waterOnMorning, int waterOffMorning, int waterOnAfternoon, int waterOffAfternoon); // Function that updates the timer display. Parameters taken are the same as timerDisplay
	void editTime(int time); // Function that display the time update screen. Takes the current time as a parameter
	void updateEditTime(int time); // Function that updates the time edit screen. Tkaes the current time as a parameter
	void loadingScreen(int NTPTryCount); // Function that displays the loading screen. Takes the current number of tries to get the time via NTP as a parameter
};

#endif

