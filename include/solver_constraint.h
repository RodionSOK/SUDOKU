#ifndef SOLVER_CONSTRAINT_H
#define SOLVER_CONSTRAINT_H

#include "sudoku_solver.h"

typedef struct {
    bool values[SIZE + 1];  
    int count;              
} CandidateSet;

typedef CandidateSet ConstraintSet[SIZE][SIZE];

bool solve_constraint(int map[SIZE][SIZE]);
void init_constraints(int map[SIZE][SIZE], ConstraintSet constraints);
// bool update_constraints(ConstraintSet constraints, int row, int col, int value);
Cell find_most_constrained_cell(int map[SIZE][SIZE], ConstraintSet constraints);

#endif