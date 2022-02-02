
import math
import typing
import numpy as np
import nptyping as npt

# (idx, lat, lon)
Point = npt.NDArray[(3,), float]

class Sphere:
    lat_start = -math.pi / 2
    lat_end = math.pi / 2
    lon_start = 0
    lon_end = math.pi * 2

    def __init__(self, dlat : float, dlon : float, length : int):
        self.dlat = dlat
        self.dlon = dlon
        self.max_len = length
        self.lat_l = math.ceil((self.lat_end - self.lat_start) / self.dlat)
        self.lon_l = math.ceil((self.lon_end - self.lon_start) / self.dlon)
        print(self.lat_l, self.lon_l)


    def calc(self, idx : int):
        # потому что точек на 1 больше чем отрезков
        lat_idx = idx % (self.lat_l + 1)
        lon_idx = idx // (self.lat_l + 1)
        #print(idx, lat_idx, lon_idx)
        lat = self.lat_start
        lon = self.lon_start
        if lat_idx == self.lat_l:
            lat = self.lat_end
        else:
            lat += self.dlat * lat_idx
        if lon_idx == self.lon_l:
            lon = self.lon_end
        else:
            lon += self.dlon * lon_idx
        return np.array([idx, lat, lon])

            



    class SphereIterator:
        def __init__(self, func : typing.Callable[[int], Point], max_p : int, length : int):
            self.func = func
            self.max_p = max_p
            self.length = length
            print(self.max_p)
            self.counter = 0

        def __next__(self):
            if self.counter * self.length < self.max_p:
                self.counter += 1
                return self.__class__.__coords_gen((self.counter-1)*self.length, min(self.max_p, self.counter*self.length), self.func)
            else:
                raise StopIteration

        def __len__(self):
            return self.length

        @staticmethod
        def __coords_gen(start : int, end : int, func : typing.Callable[[int], Point]):
            for i in range(start, end):
                yield func(i)

    def __iter__(self):
        return self.SphereIterator(self.calc, (self.lat_l+1)*(self.lon_l+1), self.max_len)


