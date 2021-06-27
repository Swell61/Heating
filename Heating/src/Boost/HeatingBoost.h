#ifndef _HEATINGBOOST_h
#define _HEATINGBOOST_h

#include "Boost.h"

class HeatingBoost final : public Boost {
    public:
        HeatingBoost(unsigned char pinNum);
};

#endif