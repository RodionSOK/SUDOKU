#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <stdbool.h>

#define SIZE 9

typedef struct {
    int row;
    int col;
} Cell;

typedef enum {
    SOLVER_BACKTRACK,
    // SOLVER_MRV,
    // SOLVER_DLX,
    SOLVER_CONSTRAINT
} SolverType;

bool validGrid(int map[SIZE][SIZE], int row, int col);
void copy_map(int src[SIZE][SIZE], int dest[SIZE][SIZE]);
void clear_map(int map[SIZE][SIZE]);
void print_map(int map[SIZE][SIZE]);

bool solve_sudoku(int map[SIZE][SIZE], SolverType type);

#endif