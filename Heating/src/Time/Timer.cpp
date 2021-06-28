// 
// 
// 

#include "Timer.h"

Timer::Timer() { }

const ComponentTimer& Timer::getHeatingTimer() const {
	return heating;
}

const ComponentTimer& Timer::getWaterTimer() const {
	return water;
}

void Timer::setHeatingTimerState(bool state) {
	heatingTimerState = state;
}
void Timer::setWaterTimerState(bool state) {
	waterTimerState = state;
}

bool Timer::adjustTimer(SystemComponent component, TimerPeriod period, State state,  ValueAdjustment adjustment) {
	switch (component) {
		case SystemComponent::HEATING: {
			return heating.adjustTimer(period, state, adjustment);
		}
		case SystemComponent::WATER: {
			return water.adjustTimer(period, state, adjustment);
		}
	}
}

Clock& Timer::getClock() {
	return clock;
}