#! /usr/bin/python
import os

fa = '/mnt/test/split_a.dat'
fb = '/mnt/test/split_b.dat'
per_size = 200 #MB
unit = 'u'*(1024*1024) #1MB

def write_sync(f, data):
    f.write(data)
    f.flush()
    f.truncate()
    os.fsync(f.fileno())

if __name__ == '__main__':

    file_a = open(fa, 'wb')
    file_b = open(fb, 'wb')
    for i in xrange(per_size):
        write_sync(file_a, unit)
        write_sync(file_b, unit)

    file_a.close()
    file_b.close()

        
        
        
         

