#ifndef TEST_PUZZLES_H
#define TEST_PUZZLES_H

#include "sudoku_solver.h"

extern int easy_puzzle[SIZE][SIZE];
extern int medium_puzzle[SIZE][SIZE];
extern int hard_puzzle[SIZE][SIZE];
extern int expert_puzzle[SIZE][SIZE];

void get_test_puzzles(int puzzles[][SIZE][SIZE], const char* names[], int* count);

#endif