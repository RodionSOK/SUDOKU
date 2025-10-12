#include "sudoku_solver.h"
#include "solver_backtrack.h"
#include "solver_mrv.h"
#include "solver_dl.h"
#include "solver_constraint.h"
#include <stdio.h>
#include <string.h>

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

bool solve_sudoku(int map[SIZE][SIZE], SolverType type) {
    bool solved = false;

    switch (type) {
        case SOLVER_BACKTRACK:
            solved = solve_backtrack(map);
            break;
        case SOLVER_MRV:
            solved = solve_mrv(map);
            break;
        case SOLVER_DLX:
            solved = solve_dl(map);
            break;
        case SOLVER_CONSTRAINT:
            solved = solve_constraint(map);
            break;
        default:
            printf("Неизвестный тип решателя\n");
            return false;
    }

    return solved;
}

void copy_map(int src[SIZE][SIZE], int dest[SIZE][SIZE]) {
    memcpy(dest, src, sizeof(int) * SIZE * SIZE);
}

void clear_map(int map[SIZE][SIZE]) {
    memset(map, 0, sizeof(int) * SIZE * SIZE);
}

void print_map(int map[SIZE][SIZE]) {
    for (int i = SIZE - 1; i >= 0; i--) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", map[j][i]);
        }
        printf("\n");
    }
}