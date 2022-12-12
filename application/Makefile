# Makefile to build the project

# Parameters
CC = gcc
CFLAGS = 

SRC = src/
BIN = bin/

RUN_ARGUMENTS = ftp://dlpuser:rNrKYTX9g7z3RgJRmxWuGHbeu@ftp.dlptest.com/test/penguin.png

# Targets

download: 
	$(CC) $(CFLAGS) $(SRC)/*.c $(SRC)/*.h -o $(BIN)download
	./$(BIN)download $(RUN_ARGUMENTS)

.PHONY: clean
clean:
	rm -f $(BIN)/download