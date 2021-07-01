#include "Water.h"

Water::Water(const ComponentTimerConfig& config, Mode initialMode) : Component(config, initialMode) { }

Water::Water(const ComponentTimerConfig& config) : Component(config) { }

bool Water::required(Clock& clock) {
    return Component::required(clock);
}