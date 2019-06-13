#include "caliper_basic.hpp"

double
CaliperBasic::euclidean(const double *p, const double *q, unsigned long n)
{
    double result = 0;
    for (int i = 0; i < n; ++i)
    {
        const double num = q[i] - p[i];
        result += num * num;
    }
    return sqrt(result);
}

double
CaliperBasic::manhattan(const double *p, const double *q, unsigned long n)
{
    double result = 0;
    for (int i = 0; i < n; ++i)
    {
        const double num = fabs(p[i] - q[i]);
        result += num;
    }
    return result;
}

double
CaliperBasic::cosine(const double *p, const double *q, unsigned long n)
{
    double top = 0;
    double left = 0;
    double right = 0;
    for (int i = 0; i < n; ++i)
    {
        const double a = p[i] * q[i];
        top += a;
        const double b = p[i] * p[i];
        left += b;
        const double c = q[i] * q[i];
        right += c;
    }
    const double bottom = sqrt(left) * sqrt(right);
    return top / bottom;
}
