#ifndef SOLVER_DL_H
#define SOLVER_DL_H

#include "sudoku_solver.h"

typedef struct DLNode {
    struct DLNode *left, *right, *up, *down;
    struct DLNode *column;
    int row, col, value;
} DLNode;

typedef struct ColumnHeader {
    DLNode node;
    int size;
} ColumnHeader;

bool solve_dl(int map[SIZE][SIZE]);
void init_dl_solver(int map[SIZE][SIZE], DLNode** root);
void cleanup_dl_solver(DLNode* root);

#endif