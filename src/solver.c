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
#include "SBPState.h"
#include "uninformed.h"
#include "heuristic.h"

// Main function
int main() {

  // Run the uninformed algorithms
  //runUninformed();

  // Run the A* algorithms
  runAStar();

  return 0;
}
