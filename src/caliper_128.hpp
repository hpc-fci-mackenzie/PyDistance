#ifndef CALIPER_128_HPP
#define CALIPER_128_HPP

#include "caliper.hpp"
#include <xmmintrin.h>

class Caliper128 : public Caliper
{
public:
	double
	euclidean(const double *, const double *, unsigned long) override;
	double
	manhattan(const double *p, const double *q, unsigned long n) override;
	double
	cosine(const double *p, const double *q, unsigned long n) override;
private:
	inline __m128d
	_mm_abs_pd(__m128d a);
	inline double
	_mm_rdcsd_f64(__m128d a);
};
#endif
