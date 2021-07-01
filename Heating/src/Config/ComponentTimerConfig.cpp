#include "ComponentTimerConfig.h"

ComponentTimerConfig::ComponentTimerConfig(const Config& config, SystemComponent component) : Config(config) {
    switch (component) {
        case SystemComponent::HEATING: {
            configReadFunc = &readHeaterTimerConfig;
			configWriteFunc = &writeHeaterTimerConfig;
            break;
        }
        case SystemComponent::WATER: {
            configReadFunc = &readWaterTimerConfig;
			configWriteFunc = &writeWaterTimerConfig;
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

bool ComponentTimerConfig::writeTimer(TimerPeriod period, State state, unsigned short int timeInMinutes) {
	return (this->*configWriteFunc)(period, state, timeInMinutes);
}

bool ComponentTimerConfig::writeHeaterTimerConfig(TimerPeriod period, State state, unsigned short int timeInMinutes) {
    switch (period) {
				case TimerPeriod::MORNING: {
					switch (state) {
						case State::ON: {
							return writeInt("heatMon", timeInMinutes);
						}
						case State::OFF: {
							return writeInt("heatMoff", timeInMinutes);
						}
					}
				}
				case TimerPeriod::AFTERNOON: {
					switch (state) {
						case State::ON: {
							return writeInt("heatAon", timeInMinutes);
						}
						case State::OFF: {
							return writeInt("heatAoff", timeInMinutes);
						}
					}
				}
			}
}

bool ComponentTimerConfig::writeWaterTimerConfig(TimerPeriod period, State state, unsigned short int timeInMinutes) {
            switch (period) {
				case TimerPeriod::MORNING: {
					switch (state) {
						case State::ON: {
							return writeInt("waterMon", timeInMinutes);
						}
						case State::OFF: {
							return writeInt("waterMoff", timeInMinutes);
						}
					}
				}
				case TimerPeriod::AFTERNOON: {
					switch (state) {
						case State::ON: {
							return writeInt("waterAon", timeInMinutes);
						}
						case State::OFF: {
							return writeInt("waterAoff", timeInMinutes);
						}
					}
				}
			}
}

unsigned short int ComponentTimerConfig::defaultReadTimerConfig(TimerPeriod period, State state) {
    return 0;
}

bool ComponentTimerConfig::defaultWriteTimerConfig(TimerPeriod period, State state, unsigned short int timeInMinutes) {
    return false;
}

unsigned char ComponentTimerConfig::readIncrement() {
	int increment = readInt("compTimeIncr", 2);
	if (increment <= UINT8_MAX && increment > 0) {
		return static_cast<unsigned char>(increment);
	}
	return 0;
}

