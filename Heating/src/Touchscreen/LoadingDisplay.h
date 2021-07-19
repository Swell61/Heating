#ifndef _LOADINGDISPLAY_h
#define _LOADINGDISPLAY_h

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "../CoreComponents.h"
#include "../Enums/SystemFunction.h"

class LoadingDisplay {
    private:

    public:
        virtual bool display(Config& config, unsigned char ntpMaxRetries, MCUFRIEND_kbv& display);
        virtual bool update(unsigned char ntpTry, MCUFRIEND_kbv& display);
};

#endif