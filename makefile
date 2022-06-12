output: main.o board.o attacks.o movegen.o 
	g++ main.o board.o attacks.o movegen.o -o output
main.o: main.cpp
	g++ -c main.cpp
board.o: board.cpp board.hpp
	g++ -c board.cpp
attacks.o: attacks.cpp attacks.hpp
	g++ -c attacks.cpp
movegen.o: movegen.cpp movegen.hpp
	g++ -c movegen.cpp
clean:
	rm *.o output