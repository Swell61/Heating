#ifndef _MAINDISPLAY_h
#define _MAINDISPLAY_h

#include "Screen.h"
#include "../Enums/SystemFunction.h"
#include "../CoreComponents.h"
#include "../Components/Heating.h"
#include "../Components/Water.h"
#include "../Enums/SystemFunction.h"
#include <TouchScreen.h>
#include "../TempSensor.h"

class MainDisplay : public Screen {
    private:
        void printHeatingMode(Heating& heating, MCUFRIEND_kbv& display);
        void printWaterMode(Water& water, MCUFRIEND_kbv& display);
        void printHeatingBoostButton(Heating& heating, Clock& clock, MCUFRIEND_kbv& display);
        void printWaterBoostButton(Water& water, Clock& clock, MCUFRIEND_kbv& display);
        void printHeatingRequired(Heating& heating, TempSensor& tempSensor, Clock& clock, MCUFRIEND_kbv& display);
        void printWaterRequired(Water& water, Clock& clock, MCUFRIEND_kbv& display);
        void printRequestedTemp(Heating& heating, MCUFRIEND_kbv& display);
        void printHeatingStatus(Heating& heating, TempSensor& tempSensor, Clock& clock, MCUFRIEND_kbv& display);
        void printWaterStatus(Water& water, Clock& clock, MCUFRIEND_kbv& display);
        void printTemperature(TempSensor& tempSensor, MCUFRIEND_kbv& display);

        void printLabels(MCUFRIEND_kbv& display);

        void printTime(Clock& clock, MCUFRIEND_kbv& display) override;
    public:
        bool display(CoreComponents& components, MCUFRIEND_kbv& display) override;
        bool update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) override;
        SystemFunction getTouchInput(TSPoint& point) override;
};

#endif