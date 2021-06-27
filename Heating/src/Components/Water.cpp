#include "Water.h"

Water::Water(Mode initialMode) : Component(initialMode) { }

Water::Water() : Component() { }

bool Water::required(Clock& clock) const {
    return Component::required(clock);
}