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
};

#endif
