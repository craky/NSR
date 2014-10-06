OBJECTS = ./objects/main.o ./objects/char_operations.o
CXXFLAGS = -Wall -pedantic 
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