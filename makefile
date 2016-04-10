all: map

map: main.o dot.o
	g++ main.o -o map -lcurl

main.o: main.cpp
	g++ -Wall -c main.cpp

dot.o: dot.cpp
	g++ -Wall -c dot.cpp

clean:
	rm *o map