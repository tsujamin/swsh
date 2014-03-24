#Use gcc to compile
CC = gcc
#Set debug symbols (-g) and all warnings (-Wall)
CFLAGS = -g -Wall -std=gnu99

#Executable name
OUT_BIN = myshell

#Sources
SRC_FILES = $(shell ls *.c) 
OBJ_FILES = $(SRC_FILES:%.c=bin/%.o)
INCLUDES = -I. -I/usr/include
LIBRARIES = -lreadline

default: prepare $(OUT_BIN) 

$(OUT_BIN): $(OBJ_FILES)
	@echo Compiling $(OUT_BIN)
	@ $(CC) $(CFLAGS) -o $(OUT_BIN) $(OBJ_FILES) $(INCLUDES) $(LIBRARIES)
	@echo Linked
prepare:
	@mkdir bin
	
bin/%.o: %.c
	@echo Compiling $<
	@ $(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@rm $(OBJ_FILES)
	@rm $(OUT_BIN)
	@echo Build directory cleaned
	
install: $(OUT_BIN)
	@echo Installing $(OUT_BIN)
	@cp $< /usr/bin/$(OUT_BIN)

uninstall:
	@echo Uninstalling $(OUT_BIN)
	@rm /usr/bin/$(OUT_BIN)
