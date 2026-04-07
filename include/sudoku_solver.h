#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <stdbool.h>

#define SIZE 16
#define BOX_SIZE 4

#define TIME_LIMIT 10

typedef struct {
    int row;
    int col;
} Cell;

typedef enum {
    SOLVER_BACKTRACK,
    SOLVER_MRV,
    SOLVER_DLX,
    SOLVER_CONSTRAINT, 
    SOLVER_BACKTRACK_16, 
    // SOLVER_MRV_16, 
    // SOLVER_DLX_16, 
    // SOLVER_CONSTRAINT_16
} SolverType;

void clear_map(int map[SIZE][SIZE]);
void print_map(int map[SIZE][SIZE]);

bool solve_sudoku(int map[SIZE][SIZE], SolverType type);

#endif