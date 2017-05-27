Sliding-Block-Puzzle
=================

Description
------------
Uses Artificial Intelligence algorithms to solve a Sliding Block Puzzle <br />
You can find an example of these types of puzzles at the following link:

http://www.novelgames.com/en/sliding/

AI Algorithms
--------
This code supports the use of the following algorithms:

  - Random Walk
  - Breadth First Search
  - Depth First Search
  - Iterative Deepening Search
  - A* with Manhattan Distance
  - A* with a modified Manhattan Distance

The modified Manhattan Distance calculates the Manhattan Distance then<br />
adds the number of adjacent blocks to the master block.  This heuristic is<br />
both dominating of pure Manhattan Distance and is admissible.

Abbreviations
--------
Use the following abbreviations when specifing the search algorithm to use:

  - Random Walk:                          rw
  - Breadth First Search:                 bfs
  - Depth First Search:                   dfs
  - Iterative Deepening Search:           ids
  - A* with Manhattan Distance:           md
  - A* with modified Manhattan Distance: mdc

The program will auto adjust capitalization, so don't worry about that.<br />
See the Usage section below for instructions on how to run the AI.

Usage
--------
The Makefile contains all of the required building and executing.<br />
The following is a walkthrough of each command:

`$ make` or `$ make compile`<br />
Will compile the code to the executable named "run". This is the default behavior.
When using this executable, run it like `$ ./run <search algorithm> <path to puzzle>`.<br />
Example: `$ ./run bfs docs/puzzle1.txt`.

`$ make execute ALG=<search algorithm> PUZZ=<path to puzzle>`<br />
Will execute the specified search algorithm (ALG) on the puzzle (PUZZ).<br />
The default is to do a Random Walk (rw) on "docs/puzzle0.txt".

`$ make write-file ALG=<serach algorithm> PUZZ=<path to puzzle>`<br />
Will execute the specified algorithm on the puzzle and write it to the file "output.txt".<br />
The default is the same as the previous.

`$ make clean`<br />
Will remove the "run" executable and the "output.txt" file.
