/* Tim Monfette (tjm354)
 * Artificial Intelligence: CS 380
 * HW1
 *
 * Set of functions for determining
 * and loading a state for the Sliding
 * Block Puzzle
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Piece struct
typedef struct Piece {
  int value;
  int topb;
  int rightb;
  int bottomb;
  int leftb;
} Piece;

// Move struct
typedef struct Move {
  Piece piece;
  char direction;
} Move;

// List of Moves
typedef struct LMoves {
  Move m;
  struct LMoves *next;
} LMoves;

// height is rows, width is columns
int width;
int height;

// Provided swapIdx function with some modifications by myself
void swapIdx(int** state, int idx1, int idx2) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if (state[i][j] == idx1) {
        state[i][j] = idx2;
      } else if (state[i][j] == idx2) {
        state[i][j] = idx1;
      }
    }
  }
}

// Create Matrix of nx rows and ny columns
int** createMatrix(int nx, int ny) {
  int** matrix;
  matrix = (int**) malloc(nx*sizeof(int*));
  for (int i = 0; i < nx; i++) {
    matrix[i] = (int*) malloc(ny*sizeof(int));
  }

  return matrix;
}

// Load a state from disk
int** loadState(char *path) {
  FILE *f;
  int** state;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int lineNum = 1;
  int rowNum = 0;

  f = fopen(path, "r");
  
  while ((read = getline(&line, &len, f)) != -1) {
    if (lineNum == 1) {
      char *wh = strtok(line, ","); 
      char *values[2];
      int i = 0;
      while (wh != NULL) {
        values[i++] = wh;
        wh = strtok(NULL, ",");
      }
      width = atoi(values[0]);
      height = atoi(values[1]);
      state = createMatrix(height, width);  
      lineNum = 2;
    } else {
      if (line[strlen(line) - 1] == '\n') {
       line[strlen(line) - 1] = '\0'; 
      } 
      char *row = strtok(line, ",");
      int col = 0;
      while (row != NULL) {
        state[rowNum][col] = atoi(row);
        col++;
        row = strtok(NULL, ",");
      }

      rowNum++;
    } 
  }

  fclose(f);

  return state;
}

// Display a state to the screen
void displayState(int** state) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      printf("%d,", state[i][j]);
    }
    printf("\n");
  }
}

// Clone a state
int** cloneState(int** state) {
  int** stateClone = createMatrix(height, width);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      stateClone[i][j] = state[i][j];
    }
  }

  return stateClone;
}

// Normalize a State
void normalizeState(int** state) {
  int nextIdx = 3;
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      if (state[i][j] == nextIdx) {
        nextIdx++;
      } else if (state[i][j] > nextIdx) {
        swapIdx(state, nextIdx, state[i][j]);
        nextIdx++;
      }
    }
  }
}

// Check if a state has been solved
bool stateSolved(int** state) {
  bool solved = true;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int currVal = state[i][j];
      if (currVal == -1) {
        solved = false;
        break;
      }
    }
    if (solved == false) {
      break;
    }
  }

  return solved;
}

// Compare two states to see if they are identical
bool compareStates(int** state1, int** state2) {
  bool same = true;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int currVal1 = state1[i][j];
      int currVal2 = state2[i][j];
      if (currVal1 != currVal2) {
        same = false;
        break;
      }
    }
    if (same == false) {
      break;
    }
  }

  return same;
}

// Generate a list of possible moves for a given piece
LMoves* generateMoves(int** state, int piece) {
  LMoves *head = NULL;
  bool found = false;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (state[i][j] != piece) {
        continue;
      } else { 
        found = true;
        Piece p; 
        p.value = piece;
        p.topb = i;
        p.bottomb = i;
        p.rightb = j;
        p.leftb = j;

        int k = 1;
        while (state[i+k][j] == piece) {
          p.bottomb = p.bottomb + 1;
          k++;
        }
        k = 1;
        while (state[i][j+k] == piece) {
          p.rightb = p.rightb + 1;
          k++;
        }

        bool allowed;
        for (int k = 0; k < 4; k++) {
          switch (k) {
            case 0:
              allowed = true;
              for (int x = p.leftb; x <= p.rightb; x++) {
                if (piece == 2) {
                  if ((state[p.topb - 1][x] != 0) && (state[p.topb - 1][x] != -1)) {
                    allowed = false;
                    break;
                  }
                } else if (state[p.topb - 1][x] != 0) {
                  allowed = false;
                  break;
                }
              }
              if (allowed == true) {
                LMoves *node = (LMoves*) malloc(sizeof(LMoves));
                Move m;
                m.piece = p;
                m.direction = 'u';
                node->m = m;
                node->next = head;
                head = node;
              }
              break;
            case 1:
              allowed = true;
              for (int x = p.leftb; x <= p.rightb; x++) {
                if (piece == 2) {
                  if ((state[p.bottomb + 1][x] != 0) && (state[p.bottomb + 1][x] != -1)) {
                    allowed = false;
                    break;
                  }
                } else if (state[p.bottomb + 1][x] != 0) {
                  allowed = false;
                  break;
                } 
              }
              if (allowed == true) {
                LMoves *node = (LMoves*) malloc(sizeof(LMoves));
                Move m;
                m.piece = p;
                m.direction = 'd';
                node->m = m;
                node->next = head;
                head = node;
              }
              break;
            case 2:
              allowed = true;
              for (int x = p.topb; x <= p.bottomb; x++) {
                if (piece == 2) {
                  if ((state[x][p.rightb + 1] != 0) && (state[x][p.rightb + 1] != -1)) {
                    allowed = false;
                    break;
                  }
                } else if (state[x][p.rightb + 1] != 0) {
                  allowed = false;
                  break;
                } 
              }
              if (allowed == true) {
                LMoves *node = (LMoves*) malloc(sizeof(LMoves));
                Move m;
                m.piece = p;
                m.direction = 'r';
                node->m = m;
                node->next = head;
                head = node;
              }
              break;
            case 3:
              allowed = true;
              for (int x = p.topb; x <= p.bottomb; x++) {
                if (piece == 2) {
                  if ((state[x][p.leftb - 1] != 0) && (state[x][p.leftb - 1] != -1)) {
                    allowed = false;
                    break;
                  }
                } else if (state[x][p.leftb - 1] != 0) {
                  allowed = false;
                  break;
                } 
              }
              if (allowed == true) {
                LMoves *node = (LMoves*) malloc(sizeof(LMoves));
                Move m;
                m.piece = p;
                m.direction = 'l';
                node->m = m;
                node->next = head;
                head = node;
              }
              break;
          }
        } 
      }
      break; 
    }
    if (found == true) {
      break;
    }
  }

  return head;
}

// Generate a list of possible moves for a state
LMoves* generateAllMoves(int** state) {
  LMoves* head = NULL; 
  int nextVal = 3;
  bool checked2 = false;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if ((state[i][j] == 2) && (checked2 == false)) {
        LMoves* movelist = generateMoves(state, 2);
        while (movelist != NULL) {
          LMoves* temp = movelist;
          movelist = movelist->next;
          temp->next = head;
          head = temp;
          checked2 = true;
        }
      } else if (state[i][j] != nextVal) {
        continue;
      } else {
        LMoves* movelist = generateMoves(state, nextVal);
        while (movelist != NULL) {
          LMoves* temp = movelist;
          movelist = movelist->next;
          temp->next = head;
          head = temp;
        }
        nextVal++;
      }
    }
  }
  
  return head;
}

// Apply a given move on a given state
void applyMove(int** state, Move m) {
  Piece p = m.piece;

  switch(m.direction) {
    case 'u': 
      for (int i = p.leftb; i <= p.rightb; i++) {
        state[p.topb - 1][i] = p.value;
        state[p.bottomb][i] = 0;
      }      
      break;
    case 'r': 
      for (int i = p.topb; i <= p.bottomb; i++) {
        state[i][p.rightb + 1] = p.value;
        state[i][p.leftb] = 0;
      }
      break;
    case 'd':
      for (int i = p.leftb; i <= p.rightb; i++) {
        state[p.bottomb + 1][i] = p.value;
        state[p.topb][i] = 0;
      }
      break;
    case 'l': 
      for (int i = p.topb; i <= p.bottomb; i++) {
        state[i][p.leftb - 1] = p.value;
        state[i][p.rightb] = 0;
      }
      break;
  }
}

// Apply a given move on a clone of a given state
int** applyMoveCloning(int** state, Move m) {
  int** newState = cloneState(state);
  applyMove(newState, m);
  
  return newState;
}

// Dispaly a move to meet instructions
void displayMove(LMoves *node) {
  switch(node->m.direction) {
    case 'u':
      printf("(%d, up)\n", node->m.piece.value);
      break;
    case 'd':
      printf("(%d, down)\n", node->m.piece.value);
      break;
    case 'l':
      printf("(%d, left)\n", node->m.piece.value);
      break;
    case 'r':
      printf("(%d, right)\n", node->m.piece.value);
      break;
  } 
}

// Get the length of the list of moves
int length(LMoves *head) {
  int length = 0;
  LMoves *current;

  for(current = head; current != NULL; current = current->next) {
    length++;
  }

  return length;
}
