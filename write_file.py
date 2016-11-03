#! /usr/bin/python
import os, string, random

letter = string.ascii_letters

fa = '/mnt/test/split_a.dat'
fb = '/mnt/test/split_b.dat'
per_size = 8 * 2**9 #MB
unit = bytearray(2**1 *1024*1024) #1MB

def rand_unit(unit):
    for i in xrange(len(unit)):
        unit[i] = random.choice(letter)

def write_sync(f, data):
    f.write(data)
    f.flush()
    f.truncate()
    os.fsync(f.fileno())

if __name__ == '__main__':
    
    rand_unit(unit)

    '''
    f = open('/mnt/test2/test1.dat', 'wb')
    for _ in xrange(8 * 2**7):
        f.write(unit)
    f.close()
    '''
    

    
    file_a = open(fa, 'wb')
    file_b = open(fb, 'wb')
    for i in xrange(per_size):
        write_sync(file_a, unit)
        write_sync(file_b, unit)

    file_a.close()
    file_b.close()
    
    
    
        
         

