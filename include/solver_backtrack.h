#ifndef SOLVER_BACKTRACK_H
#define SOLVER_BACKTRACK_H

#include "sudoku_solver.h"

typedef struct {
    Cell emptyCells[SIZE * SIZE];
    int emptyCount;
    int currentEmptyIndex;
} BacktrackSolver;

bool solve_backtrack(int map[SIZE][SIZE]);

#endif