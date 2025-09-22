#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <stdbool.h>

#define SIZE 9

typedef struct {
    int row;
    int col;
} Cell;

typedef struct {
    Cell emptyCells[SIZE * SIZE];
    int emptyCount;
    int currentEmptyIndex;
} Solver;

bool validGrid(int map[SIZE][SIZE], int row, int col);
bool solveBT(int map[SIZE][SIZE], Solver* solver);
bool solveBT_benchmark(int map[SIZE][SIZE], Solver* solver, int* iterations);

void copy_map(int src[SIZE][SIZE], int dest[SIZE][SIZE]);
void clear_map(int map[SIZE][SIZE]);
void print_map(int map[SIZE][SIZE]);

#endif