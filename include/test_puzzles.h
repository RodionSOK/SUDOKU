#ifndef TEST_PUZZLES_H
#define TEST_PUZZLES_H

#include "sudoku_solver.h"

extern int easy_puzzle[SIZE][SIZE];
extern int medium_puzzle[SIZE][SIZE];
extern int hard_puzzle[SIZE][SIZE];
extern int expert_puzzle[SIZE][SIZE];

extern int easy_puzzle_16[SIZE][SIZE];
extern int medium_puzzle_16[SIZE][SIZE];
extern int hard_puzzle_16[SIZE][SIZE];
extern int expert_puzzle_16[SIZE][SIZE];

void get_test_puzzles(int puzzles[][SIZE][SIZE]);
void get_test_puzzles_16(int puzzles[][SIZE][SIZE]);

#endif