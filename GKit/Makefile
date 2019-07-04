
PATH = /mnt/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.21.27702/bin/Hostx64/x64

CC   = "$(PATH)/cl.exe"
LINK = "$(PATH)/lib.exe"

INC_PATH += /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um"
INC_PATH += /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/ucrt"
INC_PATH += /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/shared"
INC_PATH += /I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.21.27702/include"
INC_PATH += /I "C:/Users/Jas/Documents/Libraries/SDL/include"
INC_PATH += /I ./inc/

LIB_PATH += /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64"
LIB_PATH += /LIBPATH:"C:/Users/Jas/Documents/Libraries/SDL/lib/x64"

LIBS = SDL2.lib SDL2main.lib opengl32.lib

CC_OPTIONS = /D_AMD64_ /D_HAS_EXCEPTIONS=0

LINK_OPTIONS = /IGNORE:4006

BIN = ./bin
SRC = ./src

OUTPUT = GKIT.LIB

OBJECTS = $(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(wildcard $(SRC)/*.cpp))

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CC_OPTIONS) $(INC_PATH) /Fo:$@ /c $^

$(OUTPUT): $(OBJECTS)
	$(LINK) $(LINK_OPTIONS) $(LIB_PATH) $(LIBS) $^ /OUT:$(BIN)/$@

.PHONY: clean
clean:
	/bin/rm -f $(BIN)/*.o
	/bin/rm -f $(BIN)/$(OUTPUT)
