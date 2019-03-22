#ifndef __m512D__DISTANCE_HPP
#define __m512D__DISTANCE_HPP

#include "distance_measure.hpp"
#include <immintrin.h>

class __m512d__Distance : public DistanceMeasure
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

#endif //__m512D__DISTANCE_HPP
