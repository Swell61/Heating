// 
// 
// 

#include "Timer.h"

bool Timer::getHeatingTimerStatus() {
	int time = getTime() + 1;
	if ((time > heatingOnMorning) && (time < heatingOffMorning)) {
		return true;
	}
	
	else if ((time > heatingOnAfternoon) && (time < heatingOffAfternoon)) {
		return true;
	}
	else {
		return false;
	}
}

bool Timer::getWaterTimerStatus() {
	int time = getTime() + 1;
	if ((time > waterOnMorning) && (time < waterOffMorning)) {
		return true;
	}
	
	else if (((time) > waterOnAfternoon) && (time < waterOffAfternoon)) {
		return true;

	}
	else {
		return false;
	}
}

bool Timer::setMidnight(int time) {
		if ((millis() / 60000) - time <= 1440) {
				midnight = time;
				return true;
	}
	
}

void Timer::checkMidnight() {
	if (((millis() / 60000) - midnight) >= 1440) {
		midnight = millis() / 60000;
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

int Timer::getMidnight() {
	return midnight;
}

int Timer::getTime() {
	return (millis() / 60000) - midnight;
}