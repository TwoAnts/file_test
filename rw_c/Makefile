objects = read.o util.o
read: $(objects)
	cc -o read $(objects) -pthread -lrt
read.o: util.h
	cc -c read.c
util.o: util.h 
	cc -c util.c

clean:
	rm -f *.o read
