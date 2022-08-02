

output: main.o board.o attacks.o movegen.o move.o engine.o eval.o sort.o uci.o
	g++ -O0 -g main.o board.o attacks.o movegen.o move.o engine.o eval.o sort.o uci.o -o output
main.o: main.cpp
	g++ -c -g main.cpp
board.o: board.cpp board.hpp
	g++ -c -g board.cpp
attacks.o: attacks.cpp attacks.hpp
	g++ -c -g attacks.cpp
movegen.o: movegen.cpp 
	g++ -c -g movegen.cpp
move.o: move.cpp
	g++ -c -g move.cpp
engine.o: engine.cpp engine.hpp
	g++ -c -g engine.cpp
eval.o: eval.cpp eval.hpp
	g++ -c -g eval.cpp
sort.o: sort.cpp 
	g++ -c -g sort.cpp
uci.o: uci.cpp uci.hpp
	g++ -c -g uci.cpp


clean:
	del *.o 
	del output.exe