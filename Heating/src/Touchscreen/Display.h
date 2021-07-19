// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#include "Screen.h"
#include "Arduino.h"
#include <TouchScreen.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "../CoreComponents.h"
#include "../Enums/Mode.h"
#include "../Enums/State.h"
#include "MainDisplay.h"
#include "TimerDisplay.h"
#include "TimeDisplay.h"

// This class controls the touch screen display on the thermostat. It contains the TFT components and the touch components


class Display {
private:
	MCUFRIEND_kbv display; // The LCD screen
	TouchScreen touchScreen; // The touschreen
	uint16_t xpos, ypos;  // Screen touch coordinates
	void printTime(int hours, int mins);

	Screen* currentScreen;

	MainDisplay mainDisplay;
	TimerDisplay timerDisplay;
	TimeDisplay timeDisplay;
	
public:
	Display(); // Default constructor
	SystemFunction getTouchInput(); // Function that checks the touchscreen for any presses. Returns the required operation as a byte and takes the current screen number as a parameter (The screen number determines the required button coordinates that need to be checked)
	
	bool update(CoreComponents& components, SystemFunction systemFunction);
	void loadingScreen(bool SDAvailable, int NTPTryCount); // Function that displays the loading screen. Takes the current number of tries to get the time via NTP as a parameter

	MCUFRIEND_kbv& getDisplay(); // Currently used for once time use screens (such as loading screen) to avoid unnecessary memory usage
};


#endif

