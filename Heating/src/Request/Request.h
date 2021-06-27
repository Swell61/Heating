#ifndef _REQUEST_h
#define _REQUEST_h

#include "../Components/ComponentControl.h"
#include "../Controller.h"

class Request {
    private:
        bool updateMode(unsigned char command, ComponentControl& controller);
        bool updateRequestedTemp(unsigned char command, Heating& heating);
        bool updateHeaterTimer(unsigned char command, Heating& heating);
        bool updateWaterTimer(unsigned char command, Water& water);
        bool updateBoost(unsigned char command, ComponentControl& controller);
        bool updateDisplayMode(unsigned char command, Display& display);
        bool updateTime(unsigned char command, Clock& clock);

    public:
        Request();
        bool execute(SystemFunction function, Controller& controller);
        
        static SystemFunction v1CommandToFunction(const char* data);
};

#endif