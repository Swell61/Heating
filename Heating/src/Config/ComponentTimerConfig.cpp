#include "ComponentTimerConfig.h"

ComponentTimerConfig::ComponentTimerConfig(const Config& config, SystemComponent component) : Config(config) {
    switch (component) {
        case SystemComponent::HEATING: {
            configReadFunc = &readHeaterTimerConfig;
            break;
        }
        case SystemComponent::WATER: {
            configReadFunc = &readWaterTimerConfig;
            break;
        }
    }
}

unsigned short int ComponentTimerConfig::readTimer(TimerPeriod period, State state) {
	return (this->*configReadFunc)(period, state);
}

unsigned short int ComponentTimerConfig::readHeaterTimerConfig(TimerPeriod period, State state) {
    switch (period) {
				case TimerPeriod::MORNING: {
					switch (state) {
						case State::ON: {
							return atoi(readProperty("heatMon"));
						}
						case State::OFF: {
							return atoi(readProperty("heatMoff"));
						}
					}
				}
				case TimerPeriod::AFTERNOON: {
					switch (state) {
						case State::ON: {
							return atoi(readProperty("heatAon"));
						}
						case State::OFF: {
							return atoi(readProperty("heatAoff"));
						}
					}
				}
			}
}

unsigned short int ComponentTimerConfig::readWaterTimerConfig(TimerPeriod period, State state) {
            switch (period) {
				case TimerPeriod::MORNING: {
					switch (state) {
						case State::ON: {
							return atoi(readProperty("waterMon"));
						}
						case State::OFF: {
							return atoi(readProperty("waterMoff"));
						}
					}
				}
				case TimerPeriod::AFTERNOON: {
					switch (state) {
						case State::ON: {
							return atoi(readProperty("waterAon"));
						}
						case State::OFF: {
							return atoi(readProperty("waterAoff"));
						}
					}
				}
			}
}

unsigned short int ComponentTimerConfig::defaultTimerConfig(TimerPeriod period, State state) {
    return 0;
}

unsigned char ComponentTimerConfig::readIncrement() {
	int increment = readInt("compTimeIncr", 2);
	if (increment <= UINT8_MAX && increment > 0) {
		return static_cast<unsigned char>(increment);
	}
	return 0;
}

