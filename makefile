OBJECTS = ./objects/main.o ./objects/char_operations.o ./objects/nsr_io.o \
	./objects/nsr_string.o ./objects/nsr_stack.o
CXXFLAGS = -Wall -pedantic -g
BIN = ./NSR
# allowed CXX options are 'gcc' or 'mpicc' (mpicc - Open MPI compiler)
CXX = mpicc

all: $(OBJECTS)
	@rm -f tmp/*
	@rm -rf tmp
	@make --no-print-directory compile

compile: $(BIN)

clean:
	@rm -f ./objects/*
	@rm -rf $(BIN)

$(BIN):$(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN) -lm

./objects/main.o: ./src/main.c
	@echo -n "Compiling main.c..."
	@mkdir -p log
	@mkdir -p tmp
	@$(CXX) $(CXXFLAGS) -c ./src/main.c -o ./objects/main.o -lm
	@if test -e ./tmp/main.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/main.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;

./objects/char_operations.o: ./src/char_operations.c ./src/char_operations.h
	@echo -n "Compiling char_operations.c..."
	@mkdir -p log
	@mkdir -p tmp
	@$(CXX) $(CXXFLAGS) -c ./src/char_operations.c -o ./objects/char_operations.o -lm
	@if test -e ./tmp/char_operations.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/char_operations.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;

./objects/nsr_io.o: ./src/nsr_io.c ./src/nsr_io.h
	@echo -n "Compiling nsr_io.c..."
	@mkdir -p log
	@mkdir -p tmp
	@$(CXX) $(CXXFLAGS) -c ./src/nsr_io.c -o ./objects/nsr_io.o -lm
	@if test -e ./tmp/nsr_io.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/nsr_io.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;

./objects/nsr_string.o: ./src/nsr_string.c ./src/nsr_string.h
	@echo -n "Compiling nsr_string.c..."
	@mkdir -p log
	@mkdir -p tmp
	@$(CXX) $(CXXFLAGS) -c ./src/nsr_string.c -o ./objects/nsr_string.o -lm
	@if test -e ./tmp/nsr_string.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/nsr_string.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;

./objects/nsr_stack.o: ./src/nsr_stack.c ./src/nsr_stack.h
	@echo -n "Compiling nsr_stack.c..."
	@mkdir -p log
	@mkdir -p tmp
	@$(CXX) $(CXXFLAGS) -c ./src/nsr_stack.c -o ./objects/nsr_stack.o -lm
	@if test -e ./tmp/nsr_stack.err;then tput setaf 1; echo "[ERROR]";elif test -s ./log/nsr_stack.log;then tput setaf 3;echo "[WARNING]";else tput setaf 2; echo "[OK]";fi;
	@tput sgr0;
