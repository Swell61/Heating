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
class Display {
private:
	MCUFRIEND_kbv tft;
	TouchScreen ts;
	uint16_t xpos, ypos;  //screen coordinates
	int counter = 0;
	
public:
	Display();
	void mainDisplay(byte, byte, float, bool, bool, float, bool, bool);
	byte touchUpdate(byte);
	void displayUpdate(byte, byte, float, bool, bool, float, bool, bool);
	void timerDisplay(bool, bool, int, int, int, int, int, int, int, int);
	void timerUpdate(bool, bool, int, int, int, int, int, int, int, int);
};

#endif

