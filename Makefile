#Use gcc to compile
CC = gcc
#Set debug symbols (-g) and all warnings (-Wall)
CFLAGS = -g -Wall

#Executable name
OUT_BIN = swsh

#Sources
SRC_FILES = main.c jobs.c
OBJ_FILES = $(SRC_FILES:%.c=bin/%.o)
INCLUDES = -I./includes

$(OUT_BIN): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUT_BIN) $(OBJ_FILES)
	
bin/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm $(OBJ_FILES)
	@rm $(OUT_BIN)
 