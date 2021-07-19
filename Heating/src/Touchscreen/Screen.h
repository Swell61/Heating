#ifndef _SCREEN_h
#define _SCREEN_h

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "../CoreComponents.h"
#include "../Enums/SystemFunction.h"

class Screen {

    protected:
        const static unsigned short int MIN_PRESSURE = 200;
        const static unsigned short int MAX_PRESSURE = 1000;
        virtual void printTime(Clock& clock, MCUFRIEND_kbv& display);
        void printTime(unsigned char hours, unsigned char minutes, MCUFRIEND_kbv& display);
        TSPoint getTouchPoint();

    public:
        virtual bool display(CoreComponents& components, MCUFRIEND_kbv& display) = 0;
        virtual bool update(CoreComponents& components, MCUFRIEND_kbv& display, SystemFunction function) = 0;
        virtual SystemFunction getTouchInput(TSPoint& point) = 0;
};

#endif