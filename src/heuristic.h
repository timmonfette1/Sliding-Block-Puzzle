/* Sliding Block Puzzle 
 *
 * Set of functions for creating a
 * couple heuristics to improve the
 * searching algorithms for the Sliding
 * Block Puzzle
 *
 * Code by:
 * Tim Monfette
 */

#include "mergesort.h"

// Helper function to calculate absolute value
int absoluteValue(int val) {
  if (val < 0) { return (val * -1); }
  else { return val; }
}

// Calculate Manhattan distance for a state
int manhattan(int** state) {
  int md = 0;
  int goalX, goalY, goalLoc;
  int twoX, twoY;
  int i, j, currVal;

  // Find coordinates of the first goal square
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      currVal = state[i][j];
      if (currVal == -1) {
        goalX = j;
        goalY = i;
        break;
      }
    }
  }

  // Find location of the goal (top, left, bottom, right)
  // Top = 0, left = 1, bottom = 2, right = 3
  if (goalY == 0) { goalLoc = 0; }
  else if (goalX == 0) { goalLoc = 1; }
  else if (goalY == height - 1) { goalLoc = 2; }
  else if (goalX == width - 1) { goalLoc = 3; }

  // Find coordinates of 2 Block
  bool done = false;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      currVal = state[i][j];
      if (currVal == 2) {
        if (goalLoc == 3) {         // If goal is on the right...
          int temp = j;
          while (currVal == 2) {    // use the right side of the master block
            temp++;
            currVal = state[i][temp];
          }
          temp--;
          md = md + absoluteValue(goalY - i) + absoluteValue(goalX - temp); // manhattan distance for goal on right side
        } else {
          md = md + absoluteValue(goalY - i) + absoluteValue(goalX - j);  // manhattan distance for goal anywhere else
        }
        done = true;
        break;
      }
    }
    if (done == true) { break; }
  }

  return md;
}

// Custom, admissible heuristic. Description in README
int customHeuristic(int** state) {
  int value = 0;
  int md = manhattan(state);
  int i, j, currVal, temp;
  
  // Find coordinates of the 2 block
  bool done = false;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      currVal = state[i][j];
      if (currVal == 2) {   // Once you find the master block...
        if ((i == 0) || (j == 0) || (i == height - 1) || (j == width -1))
        { return 0; }  // this returns h = 0 if the master block is even with the wall
                       // in other words, it means you're the goal state

        // Check up
        temp = i;
        while (currVal == 2) {
          temp--;
          currVal = state[temp][j];
        }
        if (currVal == 1) {}        // if adjacent to wall, do nothing
        else if (currVal == 0) {}   // if adjacent to empty block, do nothing
        else if (currVal == -1) {}  // if adjacent to goal area, do nothing
        else { value++; }           // if adjacent to another block, add 1 to heuristic

        // Check down
        temp = i;
        currVal = state[i][j];
        while (currVal == 2) {
          temp++;
          currVal = state[temp][j];
        }
        if (currVal == 1) {}
        else if (currVal == 0) {}
        else if (currVal == -1) {}
        else { value++; }

        // Check right
        temp = j;
        currVal = state[i][j];
        while (currVal == 2) {
          temp++;
          currVal = state[i][temp];
        }
        if (currVal == 1) {}
        else if (currVal == 0) {}
        else if (currVal == -1) {}
        else { value++; }

        // Check left
        temp = j;
        currVal = state[i][j];
        while (currVal == 2) {
          temp--;
          currVal = state[i][temp];
        }
        if (currVal == 1) {}
        else if (currVal == 0) {}
        else if (currVal == -1) {}
        else { value++; }

        done = true;
        break;
      }
    }
    if (done == true) { break; }
  }

  // Return number of adjacent blocks + manhattan distance
  value = value + md;

  return value;
}

// Modifed function for inserting LIFO with a heuristic
void insertLIFO_H(OpenList **oHead, ClosedList *c, LMoves *set, int** state, Open *parent, int depth, int h) {
  for (LMoves *current = set; current != NULL; current = current->next) {
    int** newState = applyMoveCloning(state, current->m);
    normalizeState(newState);
    if (checkClosed(c, newState) == true) {   // Don't insert if in CLOSED List
      continue;
    } else if (checkOpen(*oHead, newState) == true) {   // Don't insert if already in OPEN List
      continue;
    } else {    // Insert node
      OpenList *oNode = (OpenList*) malloc(sizeof(OpenList));
      Open *op = (Open*) malloc(sizeof(Open));
      op->m = current->m;
      op->state = newState;
      
      if (h == 0) {             // Use manhattan distance heuristic
        op->heuristic = 1 + depth + manhattan(newState);
      } else if (h == 1) {      // Use custom heuristic
        op->heuristic = 1 + depth + customHeuristic(newState);
      }

      op->depth = depth + 1;
      op->parent = parent;
      oNode->o = op;
      oNode->next = *oHead;   // Add to head of list
      *oHead = oNode;
    }
  }
}

// Calculate A* using Manhattan distance
// h = 0 is Manhattan Distance
// h = 1 is custom heuristic
void aStar(char* filepath, int h) {
  int depth = 0;

  // Load and normalize initial state
  int** state = loadState(filepath);
  normalizeState(state);

  if (checkInitial(state) == true) {  // Exit if initial checks returns TRUE
    return;
  }

  // Get list of moves and initialize OPEN and CLOSED Lists
  LMoves *moveSet = generateAllMoves(state);
  OpenList *oHead = NULL;
  ClosedList *c = NULL;

  // Expand root node, add to LIFO Queue, and sort the Queue
  insertLIFO_H(&oHead, c, moveSet, state, NULL, depth, h);
  insertClosed(&c, state);
  nSearched = nSearched + 1;
  MergeSort(&oHead);

  // Loop until solution or no more moves
  OpenList *temp;
  while (oHead != NULL) {
    temp = oHead;          // Pull node off of OPEN List
    oHead = temp->next;

    // Insert node into CLOSED List
    state = temp->o->state;
    insertClosed(&c, state);
    nSearched = nSearched + 1;

    // Check if it is the solution
    if (stateSolved(state) == true) {
      displaySolution(temp, state);
      return;
    }

    // Expand node, add to LIFO Queue, and sort the Queue
    depth = temp->o->depth;
    moveSet = generateAllMoves(state);
    insertLIFO_H(&oHead, c, moveSet, state, temp->o, depth, h);
    MergeSort(&oHead);
  }

  // Print final examined state if puzzle is unsolvable
  printf("No more moves to make, puzzle is unsolveable.\nLast checked state is\n");
  displayState(state);
}

// Run A* with the two heuristics
void runAStar(char* algorithm, char* filepath) {
  if (strcmp(algorithm, "md") == 0) {
    aStar(filepath, 0);
  } else {
    aStar(filepath, 1);
  }
}
