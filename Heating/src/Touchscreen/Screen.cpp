#include "Screen.h"

void Screen::printTime(Clock& clock, MCUFRIEND_kbv& display) {
    unsigned char hours = clock.getTimeInMinutes() / 60; // Calculate number of hours
	unsigned char minutes = clock.getTimeInMinutes() % 60; // Calculate number of minutes
	printTime(hours, minutes, display);
}

void Screen::printTime(unsigned char hours, unsigned char minutes, MCUFRIEND_kbv& display) {
	display.print(((hours <= 9) ? "0" : ""));
	display.print(hours);
	display.print(":");
	display.print((minutes <= 9) ? "0" : "");
	display.print(minutes); // Print the time
}