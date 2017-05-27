/* Sliding Block Puzzle
 *
 * Set of functions for performing various
 * uninformed search Algorithms on a Sliding
 * Block Puzzle
 *
 * Code by:
 * Tim Monfette
 */

#define RAND_RANGE(LO,HI) (LO + rand() / (RAND_MAX / (LO + HI) + 1))

// Global variables for nodes explored, length of solution, and execution time
int nSearched = 0;
int solLen = 0;
double execTime = 0.0;

// Perform a Random Walk for N Steps
void randomWalk(int N) {
  int moves = 0;
  srand(time(NULL));  // Seed random number generator

  // Load, normalize and display initial state
  int** state = loadState("./docs/SBP-level0.txt");
  normalizeState(state);
  printf("\n");
  displayState(state);
  printf("\n");
  LMoves *head = generateAllMoves(state);

  // Generate N number of steps
  while (moves != N) {
    int len = length("LMoves", head);
    size_t index = RAND_RANGE(0, len);
    LMoves *current = head;

    // Pick random move
    for (int i = 0; i != (int)index; i++) {
      current = current->next;
    }

    // Display move
    displayMove(current->m);
    printf("\n");
    applyMove(state, current->m);
    normalizeState(state);
    displayState(state);
    printf("\n");

    // Stop if solved
    bool solved = stateSolved(state);
    if (solved == true) {
      printf("The puzzle is solved\n");
      break;
    }

    moves++;
    head = generateAllMoves(state);
  }
}

// Print a chain of moves that led to a solution
void displaySolution(OpenList *temp, int** state) {
  LMoves *solution = NULL;

  // Add all moves to a list to trace back the solution
  for (Open *curr = temp->o; curr != NULL; curr = curr->parent) {
    LMoves *new = (LMoves*) malloc(sizeof(LMoves));
    new->m = curr->m;
    new->next = solution;
    solution = new;
  }

  // Print out all the moves
  solLen = length("LMoves", solution);
  for (LMoves *sol = solution; sol != NULL; sol = sol->next) {
    displayMove(sol->m);
  }

  // Print out final, solved state
  displayState(state);
}

// Check initial state for no moves or solved
bool checkInitial(int** state) {
  bool initial = false;

  if (stateSolved(state) == true) {   // Check if initial state is solved
    printf("The initial state is already sovled.\n");
    displayState(state);
    initial = true;
  } else {    // Check if initial state has no possible moves
    LMoves *head = generateAllMoves(state);
    if (length("LMoves", head) == 0) {
      printf("No possible moves in initial state.\nPuzzle is unsolveable\n");
      displayState(state);
      initial = true;
    }
  }

  return initial;
}

// Check the CLOSED list for a node before adding it
bool checkClosed(ClosedList *c, int** state) {
  bool visited = false;
  for (ClosedList *current = c; current != NULL; current = current->next) {
    if(compareStates(state, current->state) == true) {
      visited = true;
      break;
    }
  }  

  return visited;
}

// Check the OPEN list for a node before adding it
bool checkOpen(OpenList *oHead, int** state) {
  bool visited = false;

  for (OpenList *current = oHead; current != NULL; current = current->next) {
    if (compareStates(state, current->o->state) == true) {
      visited = true;
      break;
    }
  }

  return visited;
}

// Insert a state to the CLOSDED list
void insertClosed(ClosedList **c, int** state) {
  ClosedList *cNode = (ClosedList*) malloc(sizeof(ClosedList));
  cNode->state = state;
  cNode->next = *c;
  *c = cNode;
}

// Insert into a FIFO Queue
void insertFIFO(OpenList **oHead, OpenList **oTail, ClosedList *c, LMoves *set, int** state, Open *parent) {
  for (LMoves *current = set; current != NULL; current = current->next) {
    int** newState = applyMoveCloning(state, current->m);
    normalizeState(newState);
    if (checkClosed(c, newState) == true) {   // Don't insert if in CLOSED List
      continue;
    } else if (checkOpen(*oHead, newState) == true) {   // Don't insert if already in OPEN List
      continue;
    } else {  // Insert node into OPEN List
      OpenList *oNode = (OpenList*) malloc(sizeof(OpenList));
      Open *op = (Open*) malloc(sizeof(Open));
      op->m = current->m;
      op->state = newState;
      op->parent = parent;
      oNode->o = op;
      oNode->next = NULL;
      if (length("OpenList", *oHead) == 0) {  // If first node, update both HEAD and TAIL
        *oHead = oNode;
        *oTail = oNode;
      } else {    // Add new node to TAIL
        OpenList *temp = *oTail; 
        temp->next = oNode;
        *oTail = oNode;
      }
    }
  }
}

// Insert into a LIFO Queue
void insertLIFO(OpenList **oHead, ClosedList *c, LMoves *set, int** state, Open *parent, int depth) {
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
      op->depth = depth + 1;
      op->parent = parent;
      oNode->o = op;
      oNode->next = *oHead;   // Add to head of list
      *oHead = oNode;
    }
  }
}

// Peform a Breadth First Search
void bfs() {

  // Load and normalize initial state
  nSearched = 0;
  int** state = loadState("./docs/SBP-level1.txt");
  normalizeState(state);

  if (checkInitial(state) == true) {  // Exit if inital check returns TRUE
    return;
  } 

  // Get list of moves and initialize OPEN and CLOSED Lists
  LMoves *moveSet = generateAllMoves(state);
  OpenList *oHead = NULL;
  OpenList *oTail = NULL;
  ClosedList *c = NULL;

  // Expand root node and add to FIFO Queue
  insertFIFO(&oHead, &oTail, c, moveSet, state, NULL);
  insertClosed(&c, state);
  nSearched = nSearched + 1;

  // Loop until solution or no more moves
  OpenList *temp;
  while (oHead != NULL) {
    temp = oHead;         // Pull node off OPEN List
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

    // Expand node and add to FIFO Queue
    moveSet = generateAllMoves(state);
    insertFIFO(&oHead, &oTail, c, moveSet, state, temp->o);
  }

  // Print final examined state if puzzle is unsolvable
  printf("No more moves to make, puzzle is unsolveable.\nLast checked state is\n");
  displayState(state);
}

// Perform a Depth First Search
void dfs() {

  // Load and normalize initial state
  nSearched = 0;
  int** state = loadState("./docs/SBP-level1.txt");
  normalizeState(state);

  if (checkInitial(state) == true) {  // Exit if initial checks returns TRUE
    return;
  }
   
  // Get list of moves and initialize OPEN and CLOSED Lists
  LMoves *moveSet = generateAllMoves(state);
  OpenList *oHead = NULL;
  ClosedList *c = NULL;

  // Expand root node and add to LIFO Queue
  insertLIFO(&oHead, c, moveSet, state, NULL, 0);
  insertClosed(&c, state);
  nSearched = nSearched + 1;
  
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
    
    // Expand node and add to LIFO Queue
    moveSet = generateAllMoves(state);
    insertLIFO(&oHead, c, moveSet, state, temp->o, 0);
  }

  // Print final examined state if puzzle is unsolvable
  printf("No more moves to make, puzzle is unsolveable.\nLast checked state is\n");
  displayState(state);
}

// Function for Depth Limited Search
// Return 0 = found goal
// Return 1 = reached cutoff point
// Return 2 = no more expandable nodes (puzzle unsolvable)
int depthLimited(int** state, int depth) {
  if (depth == 0) {
    nSearched = nSearched + 1;
    if (checkInitial(state) == true) {
      return 0;   // Can also be no possible moves, but checkInitial function covers this case
    } else {
      return 1;
    }
  }

  // Initialize moves and OPEN and CLOSED Lists
  int count = 0;
  LMoves *moveSet = generateAllMoves(state);
  OpenList *oHead = NULL;
  ClosedList *c = NULL;

  // Implement with LIFO Queue
  insertLIFO(&oHead, c, moveSet, state, NULL, count);
  insertClosed(&c, state);
  nSearched = nSearched + 1;

  // Loop until solution or cutoff
  OpenList *temp;
  while ((oHead != NULL) && (count != depth)) {
    temp = oHead;   // Grab first node on OPEN List
    oHead = temp->next;
    state = temp->o->state;
    insertClosed(&c, state);
    nSearched = nSearched + 1;

    if (stateSolved(state) == true) {   // Exit with solution/success
      displaySolution(temp, state);
      return 0;
    }

    count = count + 1;
    if (count == depth) {     // Exit with cutoff
      return 1;      
    }

    // Expand node and add to LIFO Queue
    moveSet = generateAllMoves(state);
    insertLIFO(&oHead, c, moveSet, state, temp->o, count + 1);

  }

  // Exit with failure
  if ((oHead == NULL) && (count != depth)) {
    printf("No more moves to make, puzzle is unsolveable.\nLast checked state is\n");
    displayState(state);
    return 2;
  }
}

// Perform an Iterative Deepening Search
void ids() {

  // Load and normalize initial state
  nSearched = 0;
  int** state = loadState("./docs/SBP-level1.txt");
  normalizeState(state);

  // Call depth limited search repeatedly
  int depth = 0;
  while (true) { 
    int result = depthLimited(state, depth);
    if ((result == 0) || (result == 2)) { break; }  // Break out of solution or failure
    depth = depth + 1;
  }
}

// Run all of uninformed search algorithms
void runUninformed() {
  clock_t begin;
  clock_t end;

  // Perform Random Walk
  printf("======== RANDOM WALK ========\n");

  randomWalk(3);

  // Perform Breadth First Search  
  printf("======== BREADTH FIRST SEARCH ========\n");

  begin = clock();
  bfs();
  end = clock();
  execTime = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("nodes explored: %d time taken: %fs length of solution: %d\n\n", nSearched, execTime, solLen);

  // Perform Depth First Search
  printf("======== DEPTH FIRST SEARCH ========\n");

  begin = clock();
  dfs();
  end = clock();
  execTime = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("nodes explored: %d time taken: %fs length of solution: %d\n\n", nSearched, execTime, solLen);

  // Perform Iterative Deepening Search
  printf("======== ITERATIVE DEEPENING SEARCH ========\n");

  begin = clock();
  ids();
  end = clock();
  execTime = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("nodes explored: %d time taken: %fs length of solution: %d\n\n", nSearched, execTime, solLen);
}
