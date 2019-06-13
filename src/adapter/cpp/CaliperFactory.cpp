#include "CaliperFactory.hpp"

#if defined(_128)
#include "../../caliper_128.hpp"
#define METHOD Caliper128
#elif defined(_256)
#include "../../caliper_256.hpp"
#define METHOD Caliper256
#elif defined(_512)
#include "../../caliper_512.hpp"
#define METHOD Caliper512
#else
#include "../../caliper_basic.hpp"
#define METHOD CaliperBasic
#endif

Caliper *
CaliperFactory::produce()
{
    return CaliperFactory::provider<METHOD>();
}

template<class DM>
Caliper *
CaliperFactory::provider()
{
    return new DM();
}
