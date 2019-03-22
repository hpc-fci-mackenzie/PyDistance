#include "DistanceMeasureFactory.hpp"

#if defined(_0)
#include "../../distance-measure/baseline_distance.hpp"
#define METHOD BaselineDistance
#elif defined(_128)
#include "../../distance-measure/__m128d__distance.hpp"
#define METHOD __m128d__Distance
#elif defined(_256)
#include "../../distance-measure/__m256d__distance.hpp"
#define METHOD __m256d__Distance
#elif defined(_512)
#include "../../distance-measure/__m512d__distance.hpp"
#define METHOD __m512d__Distance
#else
#include "../../distance-measure/baseline_distance.hpp"
#define METHOD BaselineDistance
#endif

DistanceMeasure *
DistanceMeasureFactory::produce()
{
    return DistanceMeasureFactory::provider<METHOD>();
}

template<class DM>
DistanceMeasure *
DistanceMeasureFactory::provider()
{
    return new DM();
}
