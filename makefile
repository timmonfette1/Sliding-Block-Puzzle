# Sliding Block Puzzle Solver
# Makefile
#
# Code by:
# Tim Monfette

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
	@./run

# compiles then executes the code while writing to output file
write-file: compile
	@./run > output.txt

# clean up the executable
.PHONY: clean

clean:
	@rm ./run
