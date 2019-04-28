CC = g++
FLAGS = -Wall -g -std=c++11

INCLUDES = -IC:/Users/Jas/Documents/Libraries/Assimp/include/ -I../GKit/GKit/inc/
LINKERS  = -lassimp

SRC = mdl.cpp
EXE = mdl.exe

$(EXE): $(SRC)
	$(CC) $(INCLUDES) $(FLAGS) $^ -o $@ $(LINKERS)

run:
	./$(EXE)

debug:
	lldb $(EXE)

clean:
	rm $(EXE)
