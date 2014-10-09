OBJECTS = ./objects/main.o ./objects/char_operations.o ./objects/nsr_io.o \
	./objects/nsr_string.o
CXXFLAGS = -Wall -pedantic -g
BIN = ./NSR

all: $(OBJECTS)
	@rm -f tmp/*
	@rm -rf tmp
	@make --no-print-directory compile

compile: $(BIN)

clean:
	@rm -f ./objects/*
	@rm -rf $(BIN)

$(BIN):$(OBJECTS)
	@gcc $(CXXFLAGS) $(OBJECTS) -o $(BIN) -lm

./objects/main.o: ./src/main.c
	@echo -n "Compiling main.c..."
	@mkdir -p log
	@mkdir -p tmp
	@gcc $(CXXFLAGS) -c ./src/main.c -o ./objects/main.o -lm
	@if test -e ./tmp/main.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/main.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;

./objects/char_operations.o: ./src/char_operations.c ./src/char_operations.h
	@echo -n "Compiling char_operations.c..."
	@mkdir -p log
	@mkdir -p tmp
	@gcc $(CXXFLAGS) -c ./src/char_operations.c -o ./objects/char_operations.o -lm
	@if test -e ./tmp/char_operations.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/char_operations.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;

./objects/nsr_io.o: ./src/nsr_io.c ./src/nsr_io.h
	@echo -n "Compiling nsr_io.c..."
	@mkdir -p log
	@mkdir -p tmp
	@gcc $(CXXFLAGS) -c ./src/nsr_io.c -o ./objects/nsr_io.o -lm
	@if test -e ./tmp/char_operations.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/nsr_io.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;

./objects/nsr_string.o: ./src/nsr_string.c ./src/nsr_string.h
	@echo -n "Compiling nsr_string.c..."
	@mkdir -p log
	@mkdir -p tmp
	@gcc $(CXXFLAGS) -c ./src/nsr_string.c -o ./objects/nsr_string.o -lm
	@if test -e ./tmp/char_operations.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/nsr_string.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;
