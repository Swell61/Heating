#include "Component.h"

Component::Component(const ComponentTimerConfig& config, Mode initialMode) : mode(initialMode), boost(Boost(30)), timer(ComponentTimer(config)) { }

Component::Component(const ComponentTimerConfig& config) : Component(config, Mode::OFF) { }

Boost& Component::getBoost() {
    return boost;
}

ComponentTimer& Component::getTimer() {
    return timer;
}

bool Component::toggleMode() {
    switch (mode) {
        case Mode::ON: {
            mode = Mode::OFF;
            return true;
        }
        case Mode::OFF: {
            mode = Mode::TIMER;
            return true;
        }
        case Mode::TIMER: {
            mode = Mode::ON;
            return true;
        }
    }
}

Mode Component::getMode() const {
    return mode;
}

bool Component::required(Clock& clock) {
    return mode == Mode::ON || boost.checkState(clock) == State::ON || (mode == Mode::TIMER && timer.timerStatus(clock));
}