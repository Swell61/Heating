#include "Component.h"

Component::Component(Mode initialMode) : mode(initialMode), boost(Boost(30)) { }

Component::Component() : Component(Mode::OFF) {}

Boost& Component::getBoost() {
    return boost;
}

ComponentTimer& Component::getTimer() {
    return timer;
}

void Component::toggleMode() {
    switch (mode) {
        case Mode::ON: {
            mode = Mode::OFF;
            break;
        }
        case Mode::OFF: {
            mode = Mode::TIMER;
            break;
        }
        case Mode::TIMER: {
            mode = Mode::ON;
            break;
        }
    }
}

Mode Component::getMode() const {
    return mode;
}

bool Component::required(Clock& clock) const {
    return mode == Mode::ON || boost.checkState(clock) == State::ON || (mode == Mode::TIMER && timer.timerStatus(clock.getTimeInMinutes()));
}