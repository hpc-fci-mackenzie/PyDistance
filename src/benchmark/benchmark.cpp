#include "benchmark.hpp"

#if defined(__INTEL_COMPILER)
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif // defined(__GNUC__)

Benchmark::Benchmark(unsigned long size, unsigned long n_tests)
{

	Benchmark::size = size;
	Benchmark::n_tests = n_tests;
	Benchmark::x = (double*) _mm_malloc(32, Benchmark::size * sizeof(double));
	Benchmark::y = (double*) _mm_malloc(32, Benchmark::size * sizeof(double));

	fill_vector(Benchmark::x, Benchmark::y, Benchmark::size);
}

void
Benchmark::euclidean(Caliper *distance)
{
	StopWatch sw;

	double *x = Benchmark::x;
	double *y = Benchmark::y;
	unsigned long n = Benchmark::size;

	double time = 0.0;
	double result = 0.0;
	unsigned long do_tests = Benchmark::n_tests;

	sw.Restart();
	while (do_tests--)
	{
		distance->euclidean(x, y, n);
	}
	time = sw.ElapsedUs();

	result = distance->euclidean(x, y, n);

	std::cout << "\n" << std::endl;
	std::cout << "      METHOD:   EUCLIDEAN" << std::endl;
	print_results(distance, time, result);
}

void
Benchmark::manhattan(Caliper *distance)
{
	StopWatch sw;

	double *x = Benchmark::x;
	double *y = Benchmark::y;
	unsigned long n = Benchmark::size;

	double time = 0.0;
	double result = 0.0;
	unsigned long do_tests = Benchmark::n_tests;

	sw.Restart();
	while (do_tests--)
	{
		distance->manhattan(x, y, n);
	}
	time = sw.ElapsedUs();

	result = distance->manhattan(x, y, n);

	std::cout << "\n" << std::endl;
	std::cout << "      METHOD:   MANHATTAN" << std::endl;
	print_results(distance, time, result);
}

void
Benchmark::cosine(Caliper *distance)
{
	StopWatch sw;

	double *x = Benchmark::x;
	double *y = Benchmark::y;
	unsigned long n = Benchmark::size;

	double time = 0.0;
	double result = 0.0;
	unsigned long do_tests = Benchmark::n_tests;

	sw.Restart();
	while (do_tests--)
	{
		distance->cosine(x, y, n);
	}
	time = sw.ElapsedUs();

	result = distance->cosine(x, y, n);

	std::cout << "\n" << std::endl;
	std::cout << "      METHOD:   COSINE" << std::endl;
	print_results(distance, time, result);
}

void
Benchmark::fill_vector(double *x, double *y, unsigned long n)
{
	std::mt19937 e2;
	std::uniform_real_distribution<> dist(1, 151);

	while (n--)
	{
		x[n] = dist(e2);
		y[n] = dist(e2);
	}
}

void
Benchmark::print_results(Caliper *distance, double time, double result)
{
	std::cout << " USING CLASS:   " << distance->getClassSimpleName() << std::endl;
	std::cout.precision(12);
	std::cout << "ELAPSED TIME:   " << time / 1000000 << "s" << std::endl;
	std::cout.precision(12);
	std::cout << "      RESULT:   " << result << "\n" << std::endl;
	std::cout << "\n" << std::endl;
}
