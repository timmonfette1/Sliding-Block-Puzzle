# Sliding Block Puzzle Solver
# Makefile
#
# Code by:
# Tim Monfette

ALG="rw"
PUZZ="docs/puzzle0.txt"

# default is to compile
default: compile

# create a backup of the code
backup:
	@rm -rf .src_backup
	@cp -r src/ .src_backup

# compiles the code
compile:
	@gcc -std=c99 -o run src/solver.c

# compiles then executes the code
execute: compile
	@./run $(ALG) $(PUZZ)

# compiles then executes the code while writing to output file
write-file: compile
	@./run $(ALG) $(PUZZ) > output.txt

# clean up the executable
.PHONY: clean

clean:
	@rm -f ./run
	@rm -f ./output.txt
