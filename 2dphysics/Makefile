CC := g++
DEBUG_BUILD := -g
LANG_STD := -std=c++17
FLAGS := -Wall -Wfatal-errors $(DEBUG_BUILD) $(LANG_STD)

LINKER_FLAGS := -lm -lSDL2 -lSDL2_image -lSDL2_gfx 
LIBS := -I./libs/

OBJ_NAME := app
build:
	$(CC) $(FLAGS) $(LIBS) ./src/*.cpp ./src/Physics/*.cpp $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./app

clean:
	rm app
