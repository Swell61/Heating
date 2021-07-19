#ifndef _TIMERDISPLAY_h
#define _TIMERDISPALY_h

#include <MCUFRIEND_kbv.h>
#include "Screen.h"
#include "../CoreComponents.h"
#include "../Enums/SystemFunction.h"
#include <TouchScreen.h>

class TimerDisplay : public Screen {
    private:
        void printHeatingStatus(Heating& heating, Clock& clock, MCUFRIEND_kbv& display);
        void printWaterStatus(Water& water, Clock& clock, MCUFRIEND_kbv& display);
        void printHeatingTimer(Heating& heating, MCUFRIEND_kbv& display);
        void printWaterTimer(Water& water, MCUFRIEND_kbv& display);
        void printButtons(MCUFRIEND_kbv& display);
        void printLabels(MCUFRIEND_kbv& display);

    public:
        bool display(CoreComponents& components, MCUFRIEND_kbv& display) override;
        bool update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) override;
        SystemFunction getTouchInput(TSPoint& point) override;
};

#endif