#ifndef CALIPER_FACTORY_HPP
#define CALIPER_FACTORY_HPP
#include "../../caliper.hpp"

class CaliperFactory
{
public:
    Caliper*
    produce();
private:
    template<class DM>
    Caliper*
    provider();
};
#endif
