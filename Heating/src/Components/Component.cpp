#include "Component.h"

Component::Component(Mode initialMode) : mode(initialMode), boost(Boost(30)) { }

Component::Component() : Component(Mode::OFF) {}

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