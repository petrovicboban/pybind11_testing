import numpy as np
from cmake_example import test_module
 


def test_main() -> None:
    assert test_module.list1([1,2,3]) == [2.1, 2.0, 3.0]

    assert test_module.list2([1.23, 3.2, 5]) == [2.1, 3.2, 5] 

    a = np.array([1.0, 2.0], dtype=np.double)
    test_module.np(a)
    assert np.array_equal(a, np.array([1.0, 34.0]))

    a = np.array([1.0, 2.0], dtype=np.double)
    test_module.np2(a)
    assert np.array_equal(a, np.array([1.0, 34.0]))