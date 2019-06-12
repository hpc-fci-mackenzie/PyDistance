#include "__m256d__distance.hpp"

double
__m256d__Distance::euclidean(const double *p, const double *q, unsigned long n)
{
	double result = 0;
	__m256d euclidean = _mm256_setzero_pd();

	for (; n > 3; n -= 4)
	{
		const __m256d a = _mm256_load_pd(p);
		const __m256d b = _mm256_load_pd(q);
		const __m256d sub = _mm256_sub_pd(b, a);
		const __m256d sqr = _mm256_mul_pd(sub, sub);
		euclidean = _mm256_add_pd(euclidean, sqr);
		p += 4;
		q += 4;
	}

	result = __m256d__Distance::_mm256_rdcsd_f64(euclidean);
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
__m256d__Distance::manhattan(const double *p, const double *q, unsigned long n)
{
	double result = 0;
	__m256d manhattan = _mm256_setzero_pd();

	for (; n > 3; n -= 4)
	{
		const __m256d a = _mm256_load_pd(p);
		const __m256d b = _mm256_load_pd(q);
		const __m256d sub = _mm256_sub_pd(b, a);
		const __m256d abs = __m256d__Distance::_mm256_abs_pd(sub);
		manhattan = _mm256_add_pd(manhattan, abs);
		p += 4;
		q += 4;
	}

	result = __m256d__Distance::_mm256_rdcsd_f64(manhattan);
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
__m256d__Distance::cosine(const double *p, const double *q, unsigned long n)
{
	__m256d top = _mm256_setzero_pd();
	__m256d  left = _mm256_setzero_pd();
	__m256d  right = _mm256_setzero_pd();

	for (; n > 3; n -= 4)
	{
		const __m256d a = _mm256_load_pd(p);
		const __m256d b = _mm256_load_pd(q);
		const __m256d mul_ab = _mm256_mul_pd(a, b);
		top = _mm256_add_pd(top, mul_ab);
		const __m256d mul_aa = _mm256_mul_pd(a, a);
		left = _mm256_add_pd(left, mul_aa);
		const __m256d mul_bb = _mm256_mul_pd(b, b);
		right = _mm256_add_pd(right, mul_bb);

		p += 4;
		q += 4;
	}

	const __m128d empty = _mm_setzero_pd();
	double double_left = __m256d__Distance::_mm256_rdcsd_f64(left);
	double double_right = __m256d__Distance::_mm256_rdcsd_f64(right);

	if (n)
	{
		for (int i = 0; i < n; ++i)
		{
			const double a = p[i] * q[i];
			const __m128d top_leftover = _mm_loadl_pd(empty, &a);
			const __m256d top_leftover_256 = _mm256_castpd128_pd256(top_leftover);
			top = _mm256_add_pd(top, top_leftover_256);

			const double b = p[i] * p[i];
			double_left+= b;
			const double c = q[i] * q[i];
			double_right+= c;
		}
	}

	__m128d load_pd = _mm_loadl_pd(empty, &double_left);
	load_pd = _mm_loadh_pd(load_pd, &double_right);
	const __m128d sqrt_left_right = _mm_sqrt_pd(load_pd);

	const __m128d sqrt_right_left = _mm_shuffle_pd(sqrt_left_right, sqrt_left_right, 1);
	const __m128d bottom = _mm_mul_pd(sqrt_left_right, sqrt_right_left);

	const __m256d sum_128 = _mm256_hadd_pd(top, top);
	__m256d permute_lane = _mm256_permute2f128_pd(sum_128, sum_128, 1);

	const __m128d top_128_h = _mm256_extractf128_pd(sum_128, 1);
	const __m128d top_128_l = _mm256_extractf128_pd(permute_lane, 1);
	const __m128d top_128 = _mm_shuffle_pd(top_128_h, top_128_l, 1);

	const __m128d cosine = _mm_div_pd(top_128, bottom);
	const __m128d shuffle = _mm_shuffle_pd(cosine, cosine, 1);
	const __m128d sum = _mm_add_pd(cosine, shuffle);
	return _mm_cvtsd_f64(sum);
}

__m256d
__m256d__Distance::_mm256_abs_pd(__m256d a)
{
	static const __m256d sign_mask = _mm256_set1_pd(-0.);
	return _mm256_andnot_pd(sign_mask, a);
}

double
__m256d__Distance::_mm256_rdcsd_f64(__m256d a)
{
	__m256d sum_lane = _mm256_hadd_pd(a, a);
	__m256d permute_lane = _mm256_permute2f128_pd(sum_lane, sum_lane, 1);
	__m256d accumulator = _mm256_add_pd(sum_lane, permute_lane);
	return _mm256_cvtsd_f64(accumulator);
}
