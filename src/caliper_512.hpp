#ifndef CALIPER_512_HPP
#define CALIPER_512_HPP

#include "caliper.hpp"
#include <immintrin.h>

class Caliper512 : public Caliper
{
public:
	double
	euclidean(const double *, const double *, unsigned long) override;
	double
	manhattan(const double *p, const double *q, unsigned long n) override;
	double
	cosine(const double *p, const double *q, unsigned long n) override;
private:
	inline __m512d
	_mm512_abs_pd(__m512d a);
	inline double
	_mm512_rdcsd_f64(__m512d a);
};

#endif
