#include "../include/sudoku_solver.h"
#include <stdio.h>

bool validGrid(int map[SIZE][SIZE], int row, int col) {
    int value = map[row][col];

    for (int i = 0; i < SIZE; i++) {
        if (i != col && map[row][i] == value)
            return false;
    }

    for (int i = 0; i < SIZE; i++) {
        if (i != row && map[i][col] == value)
            return false;
    }

    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if ((i != row || j != col) && map[i][j] == value)
                return false;
        }
    }

    return true;
}

bool solveBT(int map[SIZE][SIZE], Solver* solver) {
    if (solver->emptyCount == 0) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (map[i][j] == 0) {
                    solver->emptyCells[solver->emptyCount].row = i;
                    solver->emptyCells[solver->emptyCount].col = j;
                    solver->emptyCount++;
                }
            }
        }
        solver->currentEmptyIndex = 0;
    }

    while (solver->currentEmptyIndex < solver->emptyCount) {
        Cell currentCell = solver->emptyCells[solver->currentEmptyIndex];
        int row = currentCell.row;
        int col = currentCell.col;
        bool vGrid = false;

        for (int val = map[row][col] + 1; val <= SIZE && !vGrid; val++) {
            map[row][col] = val;
            vGrid = validGrid(map, row, col);
        }

        if (vGrid) {
            solver->currentEmptyIndex++;
        } else {
            if (solver->currentEmptyIndex == 0) {
                return false;
            } else {
                map[row][col] = 0;
                solver->currentEmptyIndex--;
            }
        }
    }
    return true;
}

bool solveBT_benchmark(int map[SIZE][SIZE], Solver* solver, int* iterations) {
    *iterations = 0;
    
    if (solver->emptyCount == 0) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (map[i][j] == 0) {
                    solver->emptyCells[solver->emptyCount].row = i;
                    solver->emptyCells[solver->emptyCount].col = j;
                    solver->emptyCount++;
                }
            }
        }
        solver->currentEmptyIndex = 0;
    }

    while (solver->currentEmptyIndex < solver->emptyCount) {
        (*iterations)++;
        Cell currentCell = solver->emptyCells[solver->currentEmptyIndex];
        int row = currentCell.row;
        int col = currentCell.col;
        bool vGrid = false;

        for (int val = map[row][col] + 1; val <= SIZE && !vGrid; val++) {
            map[row][col] = val;
            vGrid = validGrid(map, row, col);
        }

        if (vGrid) {
            solver->currentEmptyIndex++;
        } else {
            if (solver->currentEmptyIndex == 0) {
                return false;
            } else {
                map[row][col] = 0;
                solver->currentEmptyIndex--;
            }
        }
    }
    return true;
}

void copy_map(int src[SIZE][SIZE], int dst[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void clear_map(int target[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            target[i][j] = 0;
        }
    }
}

void print_map(int map[SIZE][SIZE]) {
    for (int i = SIZE - 1; i >= 0; i--) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", map[j][i]);
        }
        printf("\n");
    }
}