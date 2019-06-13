import random
import math
from timeit import default_timer as timer
from scipy.spatial import distance
import PyDistance as pd


class Benchmark:
    def __init__(self, size, n_tests):
        self.size = size
        self.n_tests = n_tests
        self.x = []
        self.y = []

    def setup(self):
        random.seed(math.pi)
        self.x = [random.randint(1, 151) for _ in range(self.size)]
        self.y = [random.randint(1, 151) for _ in range(self.size)]

    def perform(self):
        start = timer()
        for i in range(0, self.n_tests):
            pd.euclidean(self.x, self.y)
        end = timer()
        time = end - start

        res = pd.euclidean(self.x, self.y)
        print("\nUSING METHOD:    " + pd.getClassName())
        self.print_results(time, res)

        start = timer()
        for i in range(0, self.n_tests):
            distance.euclidean(self.x, self.y)
        end = timer()
        time = end - start
        print("\nUSING METHOD:    SciPy")
        self.print_results(time, res)

    def print_results(self, time, result):
        if len(self.x) < 20:
            print("x -> ", self.x)
            print("y -> ", self.y)

        print("ELAPSED TIME:    ", time, "s")
        print("      RESULT:    ", result)
