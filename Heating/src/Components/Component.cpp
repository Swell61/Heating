#include "Component.h"

Component::Component(Mode initialMode) : mode(initialMode) { }

Component::Component() : Component(Mode::OFF) {}

Boost& Component::getBoost() {
    return boost;
}

ComponentTimer& Component::getTimer() {
    return timer;
}

Mode Component::getMode(Clock& clock) const {
    if (mode == Mode::ON) {
        return Mode::ON;
    }
    else if (boost.checkState(clock) == State::ON) {
        return Mode::BOOST;
    }
    return mode;
}

bool Component::required(Clock& clock) const {
    return mode == Mode::ON || boost.checkState(clock) == State::ON || (mode == Mode::TIMER && timer.timerStatus(clock.getTimeInMinutes()));
}