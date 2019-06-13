#include "Distance_C_Adapter.hpp"
#include "../../caliper_base.hpp"

Distance_type
_init_()
{
	return new BaseCaliper;
}

void
_destroy_(Distance_type untyped_ptr)
{
	auto *typed_ptr = static_cast<BaseCaliper *>(untyped_ptr);
	delete typed_ptr;
}

double
_calculate_(Distance_type untyped_self, const double *x, const double *y, unsigned long n)
{
	auto *typed_ptr = static_cast<BaseCaliper *>(untyped_self);
	return typed_ptr->euclidean(x, y, n);
}
