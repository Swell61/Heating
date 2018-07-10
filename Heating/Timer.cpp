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

bool Timer::setHeatingOnMorning(int time) {
	if (time < heatingOffMorning && time > 0) {
		heatingOnMorning = time;
		return true;
	}
	return false;
}
bool Timer::setHeatingOffMorning(int time) {
	if (time < 720 && time > heatingOnMorning) {
		heatingOffMorning = time;
		return true;
	}
	return false;
}
bool Timer::setHeatingOnAfternoon(int time) {
	if (time < heatingOffAfternoon && time > 720) {
		heatingOnAfternoon = time;
		return true;
	}
	return false;
}
bool Timer::setHeatingOffAfternoon(int time) {
	if (time < 1440 && time > heatingOnAfternoon) {
		heatingOffAfternoon = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOnMorning(int time) {
	if (time < waterOffMorning && time > 0) {
		waterOnMorning = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOffMorning(int time) {
	if (time < 720 && time > waterOnMorning) {
		waterOffMorning = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOnAfternoon(int time) {
	if (time < waterOffAfternoon && time > 720) {
		waterOnAfternoon = time;
		return true;
	}
	return false;
}
bool Timer::setWaterOffAfternoon(int time) {
	if (time < 1440 && time > waterOnAfternoon) {
		waterOffAfternoon = time;
		return true;
	}
	return false;
}

void Timer::setHeatingTimerState(bool state) {
	heatingTimerState = state;
}
void Timer::setWaterTimerState(bool state) {
	waterTimerState = state;
}