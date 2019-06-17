#include "C_Distance.h"

double
calculate(const double *x, const double *y, unsigned long n)
{
	/* The type of the C++ object */
	Distance_type object = NULL;
	double result = 0.0;

	/* invoking 'new ClassType' */
	object = _init_();

	if(!object)
	{
		return 0.0;
	}

	/* operation on the object */
	result = _calculate_(object, x, y, n);

	/* cleaning up memory */
	_destroy_(object);

	return result;
}
