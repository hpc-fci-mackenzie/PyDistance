#include "../src/benchmark/benchmark.hpp"
#include "../src/adapter/cpp/CaliperFactory.hpp"

#define EUCLIDEAN 1
#define MANHATTAN 2
#define COSINE 3

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
testNoDifference(Caliper* caliper, int distance_measure)
{
    double p[] = {1.0, 2.0, 3.0, 4.0};
    double q[] = {1.0, 2.0, 3.0, 4.0};

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

    switch(distance_measure){
        case EUCLIDEAN:
            result = caliper->euclidean(p, q, 4);
            break;
        case MANHATTAN:
            result = caliper->manhattan(p, q, 4);
            break;
        case COSINE:
            result = caliper->cosine(p, q, 4);
            break;
    }
    std::cout << std::string(20, '-');
    return result == 0;
}

int
main()
{
	auto *factory = new CaliperFactory();
	auto *caliper = factory->produce();
    int passed = 1;
    for(int i=1; i <= 3; i++)
        printf("Test %d %s\n", i+1, 
                (passed &= testNoDifference(caliper, i+1)) ? "passed" : "failed");
	return !passed;
}
