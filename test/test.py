import PyDistance as pd

from scipy.spatial.distance import euclidean, cityblock as manhattan, cosine

p = [1, 2, 3,  4,  5, 80]
q = [7, 8, 9, 10, 11, 12]

distance_matrix = {
        pd.euclidean:euclidean,
        pd.manhattan:manhattan,
        pd.cosine:cosine,
        }

for pyd_f, scipy_f in distance_matrix.items():
    res_pyd = pyd_f(p,q)
    res_scipy = scipy_f(p,q)
    assert(abs(res_pyd - res_scipy) < 0.00001)
