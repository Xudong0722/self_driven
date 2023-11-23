"""
Result:
computing
24
24
24
24
24
computing
24
CacheInfo(hits=4, misses=2, maxsize=None, currsize=2)
24
None
computing
24
"""

from functools import lru_cache

@lru_cache(maxsize=None)
def tow_num_sum(a : int, b : int) -> int:
    print('computing')
    return a + b

for i in range(0,5):
        print(tow_num_sum(7, 17))
print(tow_num_sum(17, 7))
print(tow_num_sum.cache_info())  # Get cache information
print(tow_num_sum(17, 7))
print(tow_num_sum.cache_clear()) # clear caceh
print(tow_num_sum(17, 7))