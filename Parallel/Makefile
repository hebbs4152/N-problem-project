CC=g++
CFLAGS=-I../gnuplot-iostream -lboost_iostreams -fopenmp

all: BarnesHut

Body.o: Body.cpp
	$(CC) -c $<

QuadTree.o: QuadTree.cpp
	$(CC) -c $< -fopenmp

Other.o: Other.cpp
	$(CC) -c $<

BarnesHut: main.cpp Body.o QuadTree.o Other.o
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm -f *.o BarnesHut