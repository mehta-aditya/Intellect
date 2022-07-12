output: main.o board.o attacks.o movegen.o move.o
	g++ main.o board.o attacks.o movegen.o move.o -o output
main.o: main.cpp
	g++ -c main.cpp
board.o: board.cpp board.hpp
	g++ -c board.cpp
attacks.o: attacks.cpp attacks.hpp
	g++ -c attacks.cpp
movegen.o: movegen.cpp 
	g++ -c movegen.cpp
move.o: move.cpp
	g++ -c move.cpp

clean:
	rm *.o output