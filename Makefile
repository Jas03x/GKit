
PATH = /mnt/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.24.28314/bin/Hostx64/x64

CC   = "$(PATH)/cl.exe"
LINK = "$(PATH)/lib.exe"

INC_PATH += -I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
INC_PATH += -I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
INC_PATH += -I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
INC_PATH += -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.24.28314/include"
INC_PATH += -I "C:/Users/Jas/Documents/Libraries/SDL2/include"
INC_PATH += -I ./inc/

LIB_PATH += -LIBPATH:"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64"
LIB_PATH += -LIBPATH:"C:/Users/Jas/Documents/Libraries/SDL2/lib/x64"

LIBS = SDL2.lib SDL2main.lib opengl32.lib

BIN = ./bin
SRC = ./src

OUTPUT = GKit.lib

CC_OPTIONS = -D_AMD64_ -D_HAS_EXCEPTIONS=0
LINK_OPTIONS = -IGNORE:4006

OBJECTS = $(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(wildcard $(SRC)/*.cpp))

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CC_OPTIONS) $(INC_PATH) -Fo:$@ -c $^

$(OUTPUT): $(OBJECTS)
	$(LINK) $(LINK_OPTIONS) $(LIB_PATH) $(LIBS) $^ -OUT:$(BIN)/$@

.PHONY: clean
clean:
	/bin/rm -f $(BIN)/*.o
	/bin/rm -f $(BIN)/$(OUTPUT)