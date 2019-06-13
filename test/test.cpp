#include "../src/benchmark/benchmark.hpp"
#include "../src/adapter/cpp/CaliperFactory.hpp"

int benchmark(Caliper* d_measure){
	unsigned long array_size = 11797;
	unsigned long num_tests = 100000;

	Benchmark benchmark(array_size, num_tests);
	benchmark.euclidean(d_measure);
	benchmark.manhattan(d_measure);
	benchmark.cosine(d_measure);

}

int
main()
{
	auto *factory = new CaliperFactory();
	auto *d_measure = factory->produce();

	return 0;
}
