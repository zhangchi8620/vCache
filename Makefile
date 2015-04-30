#makefile for vCache

CXX = g++
CFLAGS = -Wall -fpermissive -g

vCache:   
	$(CXX) $(CFLAGS) -o vCache main.cc 

clean:
	rm -f a.out vCache *.o
