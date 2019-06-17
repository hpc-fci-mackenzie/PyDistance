from Benchmark import Benchmark


def main():
    benchmark = Benchmark(10000, 10000)
    benchmark.setup()
    benchmark.perform()


main()
