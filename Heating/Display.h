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
#include "HeatingFunction.h"
#include "ScreenTypes.h"
#include "ComponentOperatingMode.h"
#include "Timer.h"

// This class controls the touch screen display on the thermostat. It contains the TFT components and the touch components
class Display {
private:
	MCUFRIEND_kbv tft; // The LCD screen
	TouchScreen ts; // The touschreen
	uint16_t xpos, ypos;  // Screen touch coordinates
	void printTime(int hours, int mins);
	
public:
	Display(); // Default constructor
	void mainDisplay(int time, Mode heatingMode, Mode waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost); 
	Function touchUpdate(Screen screen);
	void displayUpdate(int time, Mode heatingMode, Mode waterMode, float temp, bool heatingStatus, bool waterStatus, float requestedTemp, bool heatingBoost, bool waterBoost); 
	void timerDisplay(Mode heatingMode, Mode waterMode, const Timer& timer); 
	void timerUpdate(Mode heatingMode, Mode waterMode, const Timer& timer); 
	void updateTimerValues(const Timer& timer);
	void editTime(int time);
	void updateEditTime(int time);
	void loadingScreen(bool SDAvailable, int NTPTryCount);
};

#endif

