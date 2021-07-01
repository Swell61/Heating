#include "OnOffTimerConfig.h"

OnOffTimerConfig::OnOffTimerConfig(const ComponentTimerConfig& config, TimerPeriod period) : ComponentTimerConfig(config) {
    switch (period) {
        case TimerPeriod::MORNING: {
            configReadFunc = &readMorningTimerConfig;
            configWriteFunc = &writeMorningTimerConfig;
            break;
        }
        case TimerPeriod::AFTERNOON: {
            configReadFunc = &readAfternoonTimerConfig;
            configWriteFunc = &writeAfternoonTimerConfig;
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

unsigned short int OnOffTimerConfig::defaultReadTimerConfig(State state) {
    return 0;
}

unsigned short int OnOffTimerConfig::readTimer(State state) {
    return (this->*configReadFunc)(state);
}

bool OnOffTimerConfig::writeMorningTimerConfig(State state, unsigned short int timeInMinutes) {
    return ComponentTimerConfig::writeTimer(TimerPeriod::MORNING, state, timeInMinutes);
}

bool OnOffTimerConfig::writeAfternoonTimerConfig(State state, unsigned short int timeInMinutes) {
    return ComponentTimerConfig::writeTimer(TimerPeriod::AFTERNOON, state, timeInMinutes);
}

bool OnOffTimerConfig::defaultWriteTimerConfig(State state, unsigned short int timeInMinutes) {
    return false;
}

bool OnOffTimerConfig::writeTimer(State state, unsigned short int timeInMinutes) {
    return (this->*configWriteFunc)(state, timeInMinutes);
}