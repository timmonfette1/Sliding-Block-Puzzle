/* Sliding Block Puzzle
 *
 * Main program for the Sliding Block
 * Puzzle AI suite of algorithms
 *
 * Code by:
 * Tim Monfette
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include "SBPState.h"
#include "uninformed.h"
#include "heuristic.h"

// Main function
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Incorrect number of arguments\n");
    printf("Please refer to the README for the usage\n");
    exit(1);
  }

  clock_t begin;
  clock_t end;
  char* algorithm = argv[1];
  char* filepath = argv[2];

  for(int i = 0; algorithm[i]; i++) {
    algorithm[i] = tolower(algorithm[i]);
  }

  printf("\n======== SLIDING BLOCK PUZZLE ========\n");
  begin = clock();

  if ((strcmp(algorithm, "md") == 0) || (strcmp(algorithm, "mdc") == 0)) {
    runAStar(algorithm, filepath);
  } else if ((strcmp(algorithm, "rw") == 0) || (strcmp(algorithm, "bfs") == 0) ||
    (strcmp(algorithm, "dfs") == 0) || (strcmp(algorithm, "ids") == 0)){
    runUninformed(algorithm, filepath);
  } else {
    printf("Unrecognized search algorithm\n");
    printf("Please refer to the README for the usage\n");
    exit(1);
  }

  end = clock();
  execTime = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("nodes explored: %d time taken: %fs length of solution: %d\n\n", nSearched, execTime, solLen);

  return 0;
}
