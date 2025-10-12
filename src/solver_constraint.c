#include "solver_constraint.h"
#include <string.h>

static inline int get_box_index(int row, int col) {
    return (row / 3) * 3 + (col / 3);
}

void init_constraints(int map[SIZE][SIZE], ConstraintSet* constraints) {
    memset(constraints, 0, sizeof(ConstraintSet));
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == 0) {
                constraints->candidates[i][j].count = SIZE;
                for (int val = 1; val <= SIZE; val++) {
                    constraints->candidates[i][j].values[val] = true;
                }
            }
        }
    }
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int val = map[i][j];
            if (val != 0) {
                update_constraints(constraints, i, j, val);
            }
        }
    }
}

bool update_constraints(ConstraintSet* constraints, int row, int col, int value) {
    constraints->row[row][value] = true;
    constraints->col[col][value] = true;
    constraints->box[get_box_index(row, col)][value] = true;
    
    for (int i = 0; i < SIZE; i++) {
        if (i != col && constraints->candidates[row][i].values[value]) {
            constraints->candidates[row][i].values[value] = false;
            constraints->candidates[row][i].count--;
        }
        if (i != row && constraints->candidates[i][col].values[value]) {
            constraints->candidates[i][col].values[value] = false;
            constraints->candidates[i][col].count--;
        }
    }
    
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int r = boxRow + i;
            int c = boxCol + j;
            if ((r != row || c != col) && constraints->candidates[r][c].values[value]) {
                constraints->candidates[r][c].values[value] = false;
                constraints->candidates[r][c].count--;
            }
        }
    }
    
    return constraints->candidates[row][col].count >= 0;
}

Cell find_most_constrained_cell(int map[SIZE][SIZE], ConstraintSet* constraints) {
    Cell result = {-1, -1};
    int minCount = SIZE + 1;
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == 0 && constraints->candidates[i][j].count < minCount && 
                constraints->candidates[i][j].count > 0) {
                minCount = constraints->candidates[i][j].count;
                result.row = i;
                result.col = j;
            }
        }
    }
    
    return result;
}

bool solve_constraint(int map[SIZE][SIZE]) {
    ConstraintSet constraints;
    init_constraints(map, &constraints);
    
    Cell cell = find_most_constrained_cell(map, &constraints);
    if (cell.row == -1) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (map[i][j] == 0) return false;
            }
        }
        return true;
    }
    
    ConstraintSet temp;
    
    for (int val = 1; val <= SIZE; val++) {
        if (constraints.candidates[cell.row][cell.col].values[val]) {
            memcpy(&temp, &constraints, sizeof(ConstraintSet));
            
            map[cell.row][cell.col] = val;
            if (update_constraints(&constraints, cell.row, cell.col, val)) {
                if (solve_constraint(map)) {
                    return true;
                }
            }
            
            map[cell.row][cell.col] = 0;
            memcpy(&constraints, &temp, sizeof(ConstraintSet));
        }
    }
    
    return false;
}