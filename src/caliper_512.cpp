#include "caliper_512.hpp"

double
Caliper512::euclidean(const double *p, const double *q, unsigned long n)
{
    double result = 0;
    __m512d euclidean = _mm512_setzero_pd();

    for (; n > 7; n -= 8)
    {
        const __m512d a = _mm512_load_pd(p);
        const __m512d b = _mm512_load_pd(q);
        const __m512d sub = _mm512_sub_pd(b, a);
        const __m512d sqr = _mm512_mul_pd(sub, sub);
        euclidean = _mm512_add_pd(euclidean, sqr);
        p += 8;
        q += 8;
    }

    result = _mm512_reduce_add_pd(euclidean);
    if (n)
    {
        for (int i = 0; i < n; ++i)
        {
            const double num = q[i] - p[i];
            result += num * num;
        }
    }
    return sqrt(result);
}

double
Caliper512::manhattan(const double *p, const double *q, unsigned long n)
{
    double result = 0;
    __m512d manhattan = _mm512_setzero_pd();

    for (; n > 7; n -= 8)
    {
        const __m512d a = _mm512_load_pd(p);
        const __m512d b = _mm512_load_pd(q);
        const __m512d sub = _mm512_sub_pd(b, a);
        const __m512d abs = _mm512_abs_pd(sub);
        manhattan = _mm512_add_pd(manhattan, abs);
        p += 8;
        q += 8;
    }

    result = _mm512_reduce_add_pd(manhattan);
    if (n)
    {
        for (int i = 0; i < n; ++i)
        {
            const double num = fabs(p[i] - q[i]);
            result += num;
        }
    }
    return result;
}

double
Caliper512::cosine(const double *p, const double *q, unsigned long n)
{
    __m512d top = _mm512_setzero_pd();
    __m512d left = _mm512_setzero_pd();
    __m512d right = _mm512_setzero_pd();

    for (; n > 7; n -= 8)
    {
        const __m512d a = _mm512_load_pd(p);
        const __m512d b = _mm512_load_pd(q);
        const __m512d mul_ab = _mm512_mul_pd(a, b);
        top = _mm512_add_pd(top, mul_ab);
        const __m512d mul_aa = _mm512_mul_pd(a, a);
        left = _mm512_add_pd(left, mul_aa);
        const __m512d mul_bb = _mm512_mul_pd(b, b);
        right = _mm512_add_pd(right, mul_bb);

        p += 8;
        q += 8;
    }

    const __m256d empty = _mm256_setzero_pd();
    double double_left = _mm512_reduce_add_pd(left);
    double double_right = _mm512_reduce_add_pd(right);

    if (n)
    {
        for (int i = 0; i < n; ++i)
        {
            const double a = p[i] * q[i];
            const __m512d top_leftover = _mm512_load_pd(&a);
            top = _mm512_add_pd(top, top_leftover);
            const double b = p[i] * p[i];
            double_left += b;
            const double c = q[i] * q[i];
            double_right += c;
        }
    }

    __m256d load_pd = _mm256_loadu_pd(&double_left);
    // load_pd = _mm256_loadu_epi32_pd(load_pd, &double_right);
    const __m256d sqrt_left_right = _mm256_sqrt_pd(load_pd);

    /* const __m256d sqrt_right_left = _mm_shuffle_pd(sqrt_left_right, sqrt_left_right, 1); */
    /* const __m256d bottom = _mm_mul_pd(sqrt_left_right, sqrt_right_left); */

    /* const __m512d sum_256 = _mm512_hadd_pd(top, top); */
    /* __m512d permute_lane = _mm512_permute2f256_pd(sum_256, sum_256, 1); */

    /* const __m256d top_256_h = _mm512_extractf256_pd(sum_256, 1); */
    /* const __m256d top_256_l = _mm512_extractf256_pd(permute_lane, 1); */
    /* const __m256d top_256 = _mm_shuffle_pd(top_256_h, top_256_l, 1); */

    /* const __m256d cosine = _mm_div_pd(top_256, bottom); */
    /* const __m256d shuffle = _mm_shuffle_pd(cosine, cosine, 1); */
    /* const __m256d sum = _mm_add_pd(cosine, shuffle); */
    /* return _mm_cvtsd_f64(sum); */
    return 0.;
}
