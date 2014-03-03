#Use gcc to compile
CC = gcc
#Set debug symbols (-g) and all warnings (-Wall)
CFLAGS = -g -Wall -std=c99

#Executable name
OUT_BIN = swsh

#Sources
SRC_FILES = $(shell ls src) 
OBJ_FILES = $(SRC_FILES:%.c=bin/%.o)
INCLUDES = -I./includes


$(OUT_BIN): $(OBJ_FILES)
	@echo Swag-piling $(OUT_BIN)
	@ $(CC) $(CFLAGS) $(INCLUDES) -o $(OUT_BIN) $(OBJ_FILES)
	@echo Swag-linked
	
bin/%.o: src/%.c
	@echo Swag-piling $<
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm $(OBJ_FILES)
	@rm $(OUT_BIN)
	@echo No one shell
	@echo Should have all that swagger
