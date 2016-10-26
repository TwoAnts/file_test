#!/usr/bin/python

import random

SIZE = 200 * 1024 #KB

RQ_CNT = 300


def gen_split_array(sum, length):
    s = 0.0
    sint = 0
    sa = []
    
    for _ in xrange(length):
        rand = random.random()
        sa.append(rand)
        s += rand

    for i in xrange(length):
        sa[i] = int(sa[i] / s * sum) 
        sint += sa[i]
    
    if sint < sum:
        sa[-1] += (sum - sint)
    else:
        print 'error: sint > sum!'
    return sa

if __name__ == '__main__':
    sa = gen_split_array(SIZE, RQ_CNT)
    print sa
    print sum(sa)
    


