from random import random
from math import sqrt

def triRand(a, c, b):
    c = float(c)
    t = (c-a)/(b-a)
    y = sqrt(random())
    d = a if random() < t else b
    return d + (c-d) * y