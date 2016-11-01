#! /usr/bin/python
import os, string, random

letter = string.ascii_letters

fa = '/mnt/test/split_a.dat'
fb = '/mnt/test/split_b.dat'
per_size = 200 #MB
unit = bytearray(1024*1024) #1MB

def rand_unit(unit):
    for i in xrange(len(unit)):
        unit[i] = random.choice(letter)

def write_sync(f, data):
    f.write(data)
    f.flush()
    f.truncate()
    os.fsync(f.fileno())

if __name__ == '__main__':

    '''
    file_a = open(fa, 'wb')
    file_b = open(fb, 'wb')
    for i in xrange(per_size):
        rand_unit(unit)
        write_sync(file_a, unit)
        write_sync(file_b, unit)

    file_a.close()
    file_b.close()
    '''
    f = open('/mnt/test/test1.dat', 'wb')
    for _ in xrange(500):
        rand_unit(unit)
        f.write(unit)
    f.close()

    

        
        
        
         

