#ifndef CALIPER_ACC_BASIC_HPP
#define CALIPER_ACC_BASIC_HPP
#include "../caliper.hpp"

class CaliperAccBasic : public Caliper
{
public:
	double
	euclidean(const double *, const double *, unsigned long) override;
	double
	manhattan(const double *, const double *, unsigned long) override;
	double
	cosine(const double *p, const double *q, unsigned long n) override;
};

#endif

