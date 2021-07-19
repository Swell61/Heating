#ifndef _TIMEDISPLAY_h
#define _TIMEDISPLAY_h

#include "Screen.h"
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "../Enums/SystemFunction.h"
#include "../CoreComponents.h"

class TimeDisplay : public Screen {
    private:
        void displayTime(Clock& clock, MCUFRIEND_kbv& display);

    public:
        bool display(CoreComponents& components, MCUFRIEND_kbv& display) override;
        bool update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) override;
        SystemFunction getTouchInput(TouchScreen& touchScreen) override;
};

#endif _TIMEDISPLAY_h