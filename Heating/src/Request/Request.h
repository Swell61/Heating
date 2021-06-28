#ifndef _REQUEST_h
#define _REQUEST_h

#include "../Components/ComponentControl.h"
#include "../Components/Heating.h"
#include "../Components/Water.h"
#include "../Boost/Boost.h"
#include "../Components/Component.h"
#include "../Display.h"
#include "../Time/Clock.h"

class Controller;
class Request {
    
    public:
        Request();
        
        bool execute(SystemFunction function, Controller& controller);
        
        static SystemFunction v1CommandToFunction(unsigned char command);
};

#include "../Controller.h" // Circular dependency problem. Not too happy with this solution but it works for now

#endif