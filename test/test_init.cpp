#include "../src/benchmark/benchmark.hpp"
#include "../src/adapter/cpp/CaliperFactory.hpp"

#define EUCLIDEAN "Euclidean"
#define MANHATTAN "Manhattan"
#define COSINE    "Cosine"

#if defined(__INTEL_COMPILER)
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif // defined(__GNUC__)

using namespace std;

void 
benchmark(Caliper* d_measure)
{
	unsigned long array_size = 11797;
	unsigned long num_tests = 100000;

	Benchmark benchmark(array_size, num_tests);
	benchmark.euclidean(d_measure);
	benchmark.manhattan(d_measure);
	benchmark.cosine(d_measure);
}

/**
 * returns true if and only if the distance measured is 0
 */
int
testNoDifference(Caliper* caliper, string distance_measure)
{

    int vec_len = 4;
	double *p = (double *) _mm_malloc(32, vec_len * sizeof(double));
	double *q = (double *) _mm_malloc(32, vec_len * sizeof(double));

    for(int i=0; i < vec_len; i++)
        p[i] = q[i] = i;

    std::cout << std::string(20, '-') << std::endl;
    printf("p = [");
    for(int i=0; i < 4; i++)
        printf("%f%s", p[i], i != 3 ? ", " : "");
    printf("]\n");

    printf("q = [");
    for(int i=0; i < 4; i++)
        printf("%f%s", q[i], i != 3 ? ", " : "");
    printf("]\n");

    
    double result;
    if(distance_measure == EUCLIDEAN)
        result = caliper->euclidean(p, q, 4);
    if(distance_measure == MANHATTAN)
        result = caliper->manhattan(p, q, 4);
    if(distance_measure == COSINE)
        result = caliper->cosine(p, q, 4);
    std::cout << std::string(20, '-') << std::endl;
    printf("Result: %f\n", result);
    return result == 0;
}

int
main()
{
	auto *factory = new CaliperFactory();
	auto *caliper = factory->produce();
    int passed = 1;
    printf("Test %s %s\n", EUCLIDEAN,
            (passed &= testNoDifference(caliper, EUCLIDEAN)) ? "passed" : "failed");
    printf("Test %s %s\n", MANHATTAN, 
            (passed &= testNoDifference(caliper, MANHATTAN)) ? "passed" : "failed");
    printf("Test %s %s\n", COSINE,    
            (passed &= testNoDifference(caliper, COSINE))    ? "passed" : "failed");
	return !passed;
}
