

CC = g++
PROF=-pg
CFLAGS = -O3 -march=native
ODIR = bin
SDIR = src
TOOLDIR = tools
OFILES = board.o attacks.o engine.o eval.o move.o movegen.o sort.o uci.o misc.o main.o 
RM = del


SOURCES = $(wildcard *.cpp */*.cpp */*/*.cpp)
HEADERS = $(wildcard *.hpp */*.hpp */*/*.hpp)
OBJECTS = $(addprefix $(ODIR)/, $(OFILES))
#OBJECTS += $(TOOLDIR)/misc.o
#HEADERS = $(addprefix $(SDIR)/, $(HFILES))

TARGET = intellect
TESTING_TARGET = intellect_test
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) 
	
$(TESTING_TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TESTING_TARGET) 

$(ODIR)/%.o : $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/%.o : $(SDIR)/$(TOOLDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(ODIR)\*.o