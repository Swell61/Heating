// 
// 
// 

#include "Timer.h"

bool Timer::getHeatingTimerStatus() {
	if (!heatingTimerState) {
		return false;
	}
	else if ((millis() - midnight) > heatingOnMorning) {
		return true;
	}
	else if ((millis() - midnight) > heatingOffMorning) {
		return false;
	}
	else if ((millis() - midnight) > heatingOnAfternoon) {
		return true;
	}
	else if ((millis() - midnight) > heatingOffAfternoon) {
		return false;
	}
}

bool Timer::getWaterTimerStatus() {
	if (!waterTimerState) {
		return false;
	}
	else if ((millis() - midnight) > waterOnMorning) {
		return true;
	}
	else if ((millis() - midnight) > waterOffMorning) {
		return false;
	}
	else if ((millis() - midnight) > waterOnAfternoon) {
		return true;
	}
	else if ((millis() - midnight) > waterOffAfternoon) {
		return false;
	}
}

void Timer::setTime(int time) {
	midnight = millis() - time;
}

void Timer::checkMidnight() {
	if ((millis() - midnight) > 1439) {
		midnight = millis();
	}
}

int Timer::getHeatingOnMorning() {
	return heatingOnMorning;
}
int Timer::getHeatingOffMorning() {
	return heatingOffMorning;
}
int Timer::getHeatingOnAfternoon() {
	return heatingOnAfternoon;
}
int Timer::getHeatingOffAfternoon() {
	return heatingOffAfternoon;
}
int Timer::getWaterOnMorning() {
	return waterOnMorning;
}
int Timer::getWaterOffMorning() {
	return waterOffMorning;
}
int Timer::getWaterOnAfternoon() {
	return waterOnAfternoon;
}
int Timer::getWaterOffAfternoon() {
	return waterOffAfternoon;
}