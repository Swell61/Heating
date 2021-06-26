// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#include "Arduino.h"
#include "Clock.h"
#include "../Enums/SystemFunction.h"
#include "../Enums/TimeComponent.h"
#include "../Enums/ValueAdjustment.h"
#include "../Enums/TimerPeriod.h"
#include "../Enums/SystemComponent.h"
#include "../Enums/State.h"
#include "ComponentTimer.h"
// This class stores the properties of the timer

class Timer {
private:
	int midnight = 0; //  Variable to store midnight so the timer can keep track of whether components need to be on or off

	// Store current timer status
	bool heatingTimerState = false;
	bool waterTimerState = false;

	// Store the times for morning and afternoon on and off
	ComponentTimer heating;
	ComponentTimer water;

	Clock clock;

public:
	Timer();
	// Methods for changing timer states
	void setHeatingTimerState(bool state);
	void setWaterTimerState(bool state);

	const ComponentTimer& getHeatingTimer() const;
	const ComponentTimer& getWaterTimer() const;
	bool getHeatingTimerStatus(); // Should be in ComponentTimer but Time library is not const correct
	bool getWaterTimerStatus(); // Should be in Component but Time library is not const correct
	bool adjustTimer(SystemComponent component, TimerPeriod period, State state,  ValueAdjustment adjustment);

	Clock& getClock();
	unsigned short int getTimeInMinutes(); // Should be in clock but Time library is not const correct
};

#endif

