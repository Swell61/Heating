#include "OnOffTimerConfig.h"

OnOffTimerConfig::OnOffTimerConfig(const ComponentTimerConfig& config, TimerPeriod period) : ComponentTimerConfig(config) {
    switch (period) {
        case TimerPeriod::MORNING: {
            configReadFunc = &readMorningTimerConfig;
            break;
        }
        case TimerPeriod::AFTERNOON: {
            configReadFunc = &readAfternoonTimerConfig;
            break;
        }
    }
}

unsigned short int OnOffTimerConfig::readMorningTimerConfig(State state) {
    return ComponentTimerConfig::readTimer(TimerPeriod::MORNING, state);
}

unsigned short int OnOffTimerConfig::readAfternoonTimerConfig(State state) {
    return ComponentTimerConfig::readTimer(TimerPeriod::AFTERNOON, state);
}

unsigned short int OnOffTimerConfig::defaultTimerConfig(State state) {
    return 0;
}

unsigned short int OnOffTimerConfig::readTimer(State state) {
    return (this->*configReadFunc)(state);
}