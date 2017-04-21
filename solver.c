#include <stdio.h>
#include <time.h>
#include "SBPState.h"

#define RAND_RANGE(LO,HI) (LO + rand() / (RAND_MAX / (LO + HI) + 1))

// Perform a Random Walk for N Steps
void randomWalk(int N) {
  int moves = 0;
  srand(time(NULL));

  int** state = loadState("./example-puzzle.txt");
  normalizeState(state);
  printf("\n");
  displayState(state);
  printf("\n");
  LMoves *head = generateAllMoves(state);

  while (moves != N) {
    int len = length(head);
    srand(time(NULL));
    size_t index = RAND_RANGE(0, len);
    LMoves *current = head;

    for (int i = 0; i != (int)index; i++) {
      current = current->next;
    }

    displayMove(current); 
    printf("\n");
    applyMove(state, current->m);
    normalizeState(state);
    displayState(state);
    printf("\n");

    bool solved = stateSolved(state);
    if (solved == true) {
      printf("The puzzle is solved\n");
      break;
    }

    moves++;
    head = generateAllMoves(state);
  }
}

// Main
int main() {
  randomWalk(3);

  return 0;
}
