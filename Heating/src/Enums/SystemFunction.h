#ifndef _SYSTEMFUNCTION_h
#define _SYSTEMFUNCTION_h

enum class SystemFunction : unsigned char {
    INCREASE_REQ_TEMP = 1,
    DECREASE_REQ_TEMP = 2,
    HEATING_BOOST = 3,
    WATER_BOOST = 4,
    TIMER_DISPLAY = 5,
    MAIN_DISPLAY = 6,
    TIMER = 7,
    TIMER_ADJUST = 8,
    HEATING_MODE = 25,
    WATER_MODE = 26,
    TIME_DISPLAY = 27,
    TIME_ADJUST = 28,
    NONE
};

#endif