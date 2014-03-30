#Use gcc to compile
CC = gcc
#Set debug symbols (-g) and all warnings (-Wall)
CFLAGS = -g -Wall -std=gnu99

#Executable name
OUT_BIN = swsh

#Sources
SRC_FILES = $(shell ls src) 
OBJ_FILES = $(SRC_FILES:%.c=bin/%.o)
INCLUDES = -I./includes -I/usr/include
LIBRARIES = -lreadline

default: prepare $(OUT_BIN) 

$(OUT_BIN): $(OBJ_FILES)
	@echo Swag-piling $(OUT_BIN)
	@ $(CC) $(CFLAGS) -o $(OUT_BIN) $(OBJ_FILES) $(INCLUDES) $(LIBRARIES)
	@echo Swag-linked
	
bin/%.o: src/%.c
	@echo Swag-piling $<
	@ $(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@rm $(OBJ_FILES)
	@rm $(OUT_BIN)
	@echo No one shell
	@echo Should have all that swagger
	
install: $(OUT_BIN)
	@echo Swag-stalling $(OUT_BIN)
	@cp $< /usr/bin/$(OUT_BIN)

uninstall:
	@echo Swag-moving $(OUT_BIN)
	@rm /usr/bin/$(OUT_BIN)

prepare: 
	@mkdir bin
