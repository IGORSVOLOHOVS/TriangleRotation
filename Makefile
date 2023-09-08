C		  := gcc
C_FLAGS :=  -Wall -Wextra -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= main.dll


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(C) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -shared -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
