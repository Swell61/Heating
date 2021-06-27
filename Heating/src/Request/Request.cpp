#include "Request.h"

Request::Request() {}

bool Request::updateMode(unsigned char command, ComponentControl& controller) {
    
}

bool Request::execute(SystemFunction function, Controller& controller) {
    switch (function) {
        case SystemFunction::UP:
            controller.getComponentControl().getHeating().adjustRequestedTemp(ValueAdjustment::UP);
            return true;
        case SystemFunction::DOWN:
            controller.getComponentControl().getHeating().adjustRequestedTemp(ValueAdjustment::DOWN);
            return true;
        
        case SystemFunction::HEATING_BOOST:
            controller.getComponentControl().getHeating().getBoost().toggle();
            return true;
        case SystemFunction::WATER_BOOST:
            controller.getComponentControl().getWater().getBoost().toggle();
            return true;

        case SystemFunction::HEATING_MODE:
            controller.getComponentControl().getHeating().toggleMode();
            return true;
        case SystemFunction::HOT_WATER_MODE:
            controller.getComponentControl().getWater().toggleMode();
            return true;
        
        case SystemFunction::MAIN_DISPLAY:
        case SystemFunction::TIMER_DISPLAY:
        case SystemFunction::CURRENT_TIME_DISPLAY:
            return false; // Dsiplay needs adding
        
        case SystemFunction::SCHEDULE_HEATING_MORNING_ON_DOWN:
            return controller.getComponentControl().getHeating().getTimer().getMorningTimer().adjustTimer(State::ON, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HEATING_MORNING_ON_UP:
            return controller.getComponentControl().getHeating().getTimer().getMorningTimer().adjustTimer(State::ON, ValueAdjustment::UP);
        case SystemFunction::SCHEDULE_HEATING_MORNING_OFF_DOWN:
            return controller.getComponentControl().getHeating().getTimer().getMorningTimer().adjustTimer(State::OFF, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HEATING_MORNING_OFF_UP:
            return controller.getComponentControl().getHeating().getTimer().getMorningTimer().adjustTimer(State::OFF, ValueAdjustment::UP);

        
        case SystemFunction::SCHEDULE_HEATING_AFTERNOON_ON_DOWN:
            return controller.getComponentControl().getHeating().getTimer().getAfternoonTimer().adjustTimer(State::ON, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HEATING_AFTERNOON_ON_UP:
            return controller.getComponentControl().getHeating().getTimer().getAfternoonTimer().adjustTimer(State::ON, ValueAdjustment::UP);
        case SystemFunction::SCHEDULE_HEATING_AFTERNOON_OFF_DOWN:
            return controller.getComponentControl().getHeating().getTimer().getAfternoonTimer().adjustTimer(State::OFF, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HEATING_AFTERNOON_OFF_UP:
            return controller.getComponentControl().getHeating().getTimer().getAfternoonTimer().adjustTimer(State::OFF, ValueAdjustment::UP);

        
        case SystemFunction::SCHEDULE_HOT_WATER_MORNING_ON_DOWN:
            return controller.getComponentControl().getWater().getTimer().getMorningTimer().adjustTimer(State::ON, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HOT_WATER_MORNING_ON_UP:
            return controller.getComponentControl().getWater().getTimer().getMorningTimer().adjustTimer(State::ON, ValueAdjustment::UP);
        case SystemFunction::SCHEDULE_HOT_WATER_MORNING_OFF_DOWN:
            return controller.getComponentControl().getWater().getTimer().getMorningTimer().adjustTimer(State::OFF, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HOT_WATER_MORNING_OFF_UP:
            return controller.getComponentControl().getWater().getTimer().getMorningTimer().adjustTimer(State::OFF, ValueAdjustment::UP);

        
        case SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_ON_DOWN:
            return controller.getComponentControl().getWater().getTimer().getAfternoonTimer().adjustTimer(State::ON, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_ON_UP:
            return controller.getComponentControl().getWater().getTimer().getAfternoonTimer().adjustTimer(State::ON, ValueAdjustment::UP);
        case SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_OFF_DOWN:
            return controller.getComponentControl().getWater().getTimer().getAfternoonTimer().adjustTimer(State::OFF, ValueAdjustment::DOWN);
        case SystemFunction::SCHEDULE_HOT_WATER_AFTERNOON_OFF_UP:
            return controller.getComponentControl().getWater().getTimer().getAfternoonTimer().adjustTimer(State::OFF, ValueAdjustment::UP);


        
        case SystemFunction::INCREASE_TENS_HOURS:
            return controller.getClock().adjustTime(TimeComponent::HOURS_TENS, ValueAdjustment::UP);
        case SystemFunction::DECREASE_TENS_HOURS:
            return controller.getClock().adjustTime(TimeComponent::HOURS_TENS, ValueAdjustment::DOWN);
        case SystemFunction::INCREASE_HOURS:
            return controller.getClock().adjustTime(TimeComponent::HOURS_ONES, ValueAdjustment::UP);
        case SystemFunction::DECREASE_HOURS:
            return controller.getClock().adjustTime(TimeComponent::HOURS_ONES, ValueAdjustment::DOWN);
        
        case SystemFunction::INCREASE_TENS_MINUTES:
            return controller.getClock().adjustTime(TimeComponent::MINUTES_TENS, ValueAdjustment::UP);
        case SystemFunction::DECREASE_TENS_MINUTES:
            return controller.getClock().adjustTime(TimeComponent::MINUTES_TENS, ValueAdjustment::DOWN);
        case SystemFunction::INCREASE_MINUTES:
            return controller.getClock().adjustTime(TimeComponent::MINUTES_ONES, ValueAdjustment::UP);
        case SystemFunction::DECREASE_MINUTES:
            return controller.getClock().adjustTime(TimeComponent::MINUTES_ONES, ValueAdjustment::DOWN);
    };
}

SystemFunction Request::v1CommandToFunction(const char* data) {
	unsigned char command = atoi(data); // Return it
	return (command < static_cast<unsigned char>(SystemFunction::END) && command >= 0) ? static_cast<SystemFunction>(command) : SystemFunction::NONE;
						
}
