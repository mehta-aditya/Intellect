

output: main.o board.o attacks.o movegen.o move.o engine.o eval.o
	g++ -Wall -Wextra -Werror -ggdb -O0 -g main.o board.o attacks.o movegen.o move.o engine.o eval.o -o output
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
engine.o: engine.cpp engine.hpp
	g++ -c engine.cpp
eval.o: eval.cpp eval.hpp
	g++ -c eval.cpp

clean:
	rm *.o output