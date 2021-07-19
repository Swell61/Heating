#include "TimerDisplay.h"

bool TimerDisplay::display(CoreComponents& components, MCUFRIEND_kbv& display) {
	display.fillScreen(0x00000); // Fill the screen with black
	printLabels(display);
	printButtons(display);

	display.setTextSize(3);
	printHeatingStatus(components.getComponentControl().getHeating(), components.getClock(), display);
	printWaterStatus(components.getComponentControl().getWater(), components.getClock(), display);

	display.setTextSize(2);
	printHeatingTimer(components.getComponentControl().getHeating(), display);

    printWaterTimer(components.getComponentControl().getWater(), display);
	
}


bool TimerDisplay::update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) {
    // The code in this function does what timerDisplay does, but it overwrites with the new status
	display.setTextSize(3);
	printHeatingStatus(components.getComponentControl().getHeating(), components.getClock(), display);
	printWaterStatus(components.getComponentControl().getWater(), components.getClock(), display);

	display.setTextSize(2);
	printHeatingTimer(components.getComponentControl().getHeating(), display);

    printWaterTimer(components.getComponentControl().getWater(), display);
}

void TimerDisplay::printHeatingStatus(Heating& heating, Clock& clock, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF); // Set text colour to white
	display.fillRect(145, 5, 56, 25, heating.getTimer().timerStatus(clock) ? 0x7CFC0 : 0xFF000); // Create rectangle for heating timer status. If heating timer is on, fill green, otherwise fill red
	display.setCursor(150, 10); // Set the cursor to write text to the display
	display.println(F("HEAT")); // Print label
}

void TimerDisplay::printWaterStatus(Water& water, Clock& clock, MCUFRIEND_kbv& display) {
	display.setTextColor(0xFFFFF); // Set text colour to white
	display.fillRect(295, 5, 68, 25, water.getTimer().timerStatus(clock) ? 0x7CFC0 : 0xFF000); // Create rectangle for hot water timer status. If hot water timer is on, fill green, otherwise fill red
	display.setCursor(300, 10); // Set the cursor to write text to the display
	display.println(F("WATER")); // Print label
}

void TimerDisplay::printHeatingTimer(Heating& heating, MCUFRIEND_kbv& display) {
	unsigned char hours;
	unsigned char minutes;

	// Print morning on for heating
	display.setCursor(145, 50);
	hours = (heating.getTimer().getMorningTimer().getOnTime() / 60);
	minutes = (heating.getTimer().getMorningTimer().getOnTime() % 60);
	printTime(hours, minutes, display);


	// Print morning off for heating
	display.setCursor(145, 100);
	hours = (heating.getTimer().getMorningTimer().getOffTime() / 60);
	minutes = (heating.getTimer().getMorningTimer().getOffTime() % 60);
	printTime(hours, minutes, display);

	// Print afternoon on for heating
	display.setCursor(145, 150);
	hours = (heating.getTimer().getAfternoonTimer().getOnTime() / 60);
	minutes = (heating.getTimer().getAfternoonTimer().getOnTime() % 60);
	printTime(hours, minutes, display);

	// Print afternoon off for heating
	display.setCursor(145, 200);
	hours = (heating.getTimer().getAfternoonTimer().getOffTime() / 60);
	minutes = (heating.getTimer().getAfternoonTimer().getOffTime() % 60);
	printTime(hours, minutes, display);
}

void TimerDisplay::printWaterTimer(Water& water, MCUFRIEND_kbv& display) {
	unsigned char hours;
	unsigned char minutes;
	// Print morning on for water
	display.setCursor(300, 50);
	hours = (water.getTimer().getMorningTimer().getOnTime() / 60);
	minutes = (water.getTimer().getMorningTimer().getOnTime() % 60);
	printTime(hours, minutes, display);

	// Print morning off for water
	display.setCursor(300, 100);
	hours = (water.getTimer().getMorningTimer().getOffTime() / 60);
	minutes = (water.getTimer().getMorningTimer().getOffTime() % 60);
	printTime(hours, minutes, display);

	// Print afternoon on for water
	display.setCursor(300, 150);
	hours = (water.getTimer().getAfternoonTimer().getOnTime() / 60);
	minutes = (water.getTimer().getAfternoonTimer().getOnTime() % 60);
	printTime(hours, minutes, display);

	// Print afternoon off for water
	display.setCursor(300, 200);
	hours = (water.getTimer().getAfternoonTimer().getOffTime() / 60);
	minutes = (water.getTimer().getAfternoonTimer().getOffTime() % 60);
	printTime(hours, minutes, display);
}

void TimerDisplay::printLabels(MCUFRIEND_kbv& display) {
	// Print the labels for each time
	display.setTextSize(1);
	display.setCursor(0, 50);
	display.println(F("Morning on:"));
	display.setCursor(0, 100);
	display.println(F("Morning off:"));
	display.setCursor(0, 150);
	display.println(F("Afternoon on:"));
	display.setCursor(0, 200);
	display.println(F("Afternoon off:"));
}

void TimerDisplay::printButtons(MCUFRIEND_kbv& display) {
	// Print the plus and minus buttons for heating
	for (unsigned char y = 50; y <= 200; y += 50) {
		display.setCursor(120, y);
		display.print(F("-"));
		display.setCursor(215, y);
		display.print(F("+"));
		y = y + 50;
	}

	// Print the plus and minus buttons for hot water
	for (unsigned char y = 50; y <= 200; y += 50) {
		display.setCursor(275, y);
		display.print("-");
		display.setCursor(370, y);
		display.print(F("+"));
		y = y + 50;
	}

	display.fillRect(0, 0, 80, 20, 0xFFFFF); // Fill a rectangle for the back button
	display.setTextColor(0x00000); // Set text colour to black
	display.setCursor(5, 0); // Set the cursor to write text to the display
	display.println(F("BACK")); // Print the button label
}


SystemFunction TimerDisplay::getTouchInput(TouchScreen& touchScreen) {
	TSPoint tp = touchScreen.getPoint();

	if (tp.z > MIN_PRESSURE && tp.z < MAX_PRESSURE) {
		if (tp.x >= 820 && tp.x <= 890 && tp.y >= 200 && tp.y <= 320) { // Back button
			return SystemFunction::MAIN_DISPLAY;
		}
		else if (tp.x >= 660 && tp.x <= 775 && tp.y >= 360 && tp.y <= 400) {
			return SystemFunction::SCHEDULE_HEATING_MORNING_ON_DOWN;
		}
		else if (tp.x >= 660 && tp.x <= 775 && tp.y >= 540 && tp.y <= 620) {
			return SystemFunction::SCHEDULE_HEATING_MORNING_ON_UP;
		}
		else if (tp.x >= 530 && tp.x <= 610 && tp.y >= 360 && tp.y <= 400) {
			return SystemFunction::SCHEDULE_HEATING_MORNING_OFF_DOWN; // Decrease heating morning off by 5 minutes
		}
		else if (tp.x >= 530 && tp.x <= 610 && tp.y >= 540 && tp.y <= 620) {
			return SystemFunction::SCHEDULE_HEATING_MORNING_OFF_UP; // Increase heating morning off by 5 minutes
		}
		else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 360 && tp.y <= 400) {
			return SystemFunction::SCHEDULE_HEATING_AFTERNOON_ON_DOWN; // Decrease heating afternoon on by 5 minutes
		}
		else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 540 && tp.y <= 620) {
			return SystemFunction::SCHEDULE_HEATING_AFTERNOON_ON_UP; // Increase heating afternoon on by 5 minutes
		}
		else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 360 && tp.y <= 400) {
			return SystemFunction::SCHEDULE_HEATING_AFTERNOON_OFF_DOWN; // Decrease heating afternoon off by 5 minutes
		}
		else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 540 && tp.y <= 620) {
			return SystemFunction::SCHEDULE_HEATING_AFTERNOON_OFF_UP; // Increase heating afternoon off by 5 minutes
		}

		else if (tp.x >= 685 && tp.x <= 778 && tp.y >= 650 && tp.y <= 730) {
			return SystemFunction::SCHEDULE_HOT_WATER_MORNING_ON_DOWN; // Decrease water morning on by 5 minutes
		}
		else if (tp.x >= 685 && tp.x <= 778 && tp.y >= 850 && tp.y <= 910) {
			return SystemFunction::SCHEDULE_HOT_WATER_MORNING_ON_UP; // Increase water morning on by 5 minutes
		}
		else if (tp.x >= 515 && tp.x <= 620 && tp.y >= 650 && tp.y <= 730) {
			return SystemFunction::SCHEDULE_HOT_WATER_MORNING_OFF_DOWN; // Decrease water morning off by 5 minutes
		}
		else if (tp.x >= 515 && tp.x <= 620 && tp.y >= 850 && tp.y <= 910) {
			return SystemFunction::SCHEDULE_HOT_WATER_MORNING_OFF_UP; // Increase water morning off by 5 minutes
		}
		else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 650 && tp.y <= 730) {
			return SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_ON_DOWN; // Decrease water afternoon on by 5 minutes
		}
		else if (tp.x >= 375 && tp.x <= 470 && tp.y >= 850 && tp.y <= 910) {
			return SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_ON_UP; // Increase water afternoon on by 5 minutes
		}
		else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 650 && tp.y <= 730) {
			return SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_OFF_DOWN; // Decrease water afternoon off by 5 minutes
		}
		else if (tp.x >= 215 && tp.x <= 310 && tp.y >= 850 && tp.y <= 910) {
			return SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_OFF_UP; // Increase water afternoon off by 5 minutes
		}
	}
	return SystemFunction::NONE;
}