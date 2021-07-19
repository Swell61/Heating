#include "TimeDisplay.h"

bool TimeDisplay::display(CoreComponents& components, MCUFRIEND_kbv& display) {

	display.fillScreen(0x00000); // Fill the screen black
	
	displayTime(components.getClock(), display);

	display.setTextSize(4); // Decrease text size to 4

	// Print the plus and minus adjust buttons
	display.setCursor(100, 30);
	display.print(F("+"));
	display.setCursor(145, 30);
	display.print(F("+"));
	display.setCursor(227, 30);
	display.print(F("+")); 
	display.setCursor(272, 30);
	display.print(F("+"));

	display.setCursor(100, 185);
	display.print(F("-"));
	display.setCursor(145, 185);
	display.print(F("-"));
	display.setCursor(227, 185);
	display.print(F("-"));
	display.setCursor(272, 185);
	display.print(F("-"));

	display.setTextSize(3); // Decrease text size to 3

	display.fillRect(0, 0, 80, 20, 0xFFFFF); // Create rectangle for back button and fill in white
	display.setTextColor(0x00000); // Set text colour to black
	display.setCursor(5, 0); // Set the cursor to write text to the display
	display.println(F("BACK")); // Print button label
}

void TimeDisplay::displayTime(Clock& clock, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF); // Set tetx colour to white
	display.setTextSize(7); // Increase text size to 7
	display.setCursor(95, 100); // Set the cursor to write text to the display
	printTime(clock, display);
}

bool TimeDisplay::update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) {
    displayTime(components.getClock(), display);
}

SystemFunction TimeDisplay::getTouchInput(TouchScreen& touchScreen) {
	TSPoint tp = touchScreen.getPoint();

	if (tp.z > MIN_PRESSURE && tp.z < MAX_PRESSURE) {
		if (tp.x >= 820 && tp.x <= 890 && tp.y >= 200 && tp.y <= 320) { // Back button
			return SystemFunction::MAIN_DISPLAY;
		}
		if (tp.x >= 715 && tp.x <= 800 && tp.y >= 320 && tp.y <= 380) {
			return SystemFunction::INCREASE_TENS_HOURS; // Increase 10 hours
		}
		if (tp.x >= 240 && tp.x <= 340 && tp.y >= 320 && tp.y <= 380) {
			return SystemFunction::DECREASE_TENS_HOURS; // Decrease 10 hours
		}
		if (tp.x >= 715 && tp.x <= 800 && tp.y >= 400 && tp.y <= 480) {
			return SystemFunction::INCREASE_HOURS; // Increase 1 hour
		}
		if (tp.x >= 240 && tp.x <= 340 && tp.y >= 400 && tp.y <= 480) {
			return SystemFunction::DECREASE_HOURS; // Decrease 1 hour
		}
		if (tp.x >= 715 && tp.x <= 800 && tp.y >= 570 && tp.y <= 645) {
			return SystemFunction::INCREASE_TENS_MINUTES; // Increase 10 minutes
		}
		if (tp.x >= 240 && tp.x <= 340 && tp.y >= 570 && tp.y <= 645) {
			return SystemFunction::DECREASE_TENS_MINUTES; // Decrease 10 minutes
		}
		if (tp.x >= 715 && tp.x <= 800 && tp.y >= 665 && tp.y <= 735) {
			return SystemFunction::INCREASE_MINUTES; // Increase 1 minute
		}
		if (tp.x >= 240 && tp.x <= 340 && tp.y >= 665 && tp.y <= 735) {
			return SystemFunction::DECREASE_MINUTES; // Decrease 1 minute
		}
	}
	return SystemFunction::NONE;
}