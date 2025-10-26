#include "solver_constraint.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

// МНЗ Сделана inline для уменьшения накладных расходов на вызов функции
static inline bool update_constraints(ConstraintSet constraints, int row, int col, int value) {
    
    // МНЗ Кэшируем указатели на строку и колонку для ускорения доступа
    // for (int i = 0; i < SIZE; i++) {
    //     if (i != col && constraints[row][i].values[value]) {
    //         constraints[row][i].values[value] = false;
    //         constraints[row][i].count--;
    //     }

    CandidateSet* rowCells = constraints[row];
    
    for (int i = 0; i < SIZE; i++) {
        if (i != col && rowCells[i].values[value]) {
            rowCells[i].values[value] = false;
            rowCells[i].count--;
        }
        // if (i != row && constraints[i][col].values[value]) {
        //     constraints[i][col].values[value] = false;
        //     constraints[i][col].count--;
        // }

        // МНЗ Вместо повторных обращений к constraints[i][col]
        CandidateSet* colCell = &constraints[i][col];
        if (i != row && colCell->values[value]) {
            colCell->values[value] = false;
            colCell->count--;
        }
    }
    
    int boxRow = (row / BOX_SIZE) * BOX_SIZE;
    int boxCol = (col / BOX_SIZE) * BOX_SIZE;
    for (int i = 0; i < BOX_SIZE; i++) {

        // МНЗ Вместо повторных обращений к constraints[r][c]
        // for (int j = 0; j < 3; j++) {
        //     int r = boxRow + i;
        //     int c = boxCol + j;
        //     if ((r != row || c != col) && constraints[r][c].values[value]) {
        //         constraints[r][c].values[value] = false;
        //         constraints[r][c].count--;
        //     }
        // }

        int r = boxRow + i;
        CandidateSet* boxRowCells = constraints[r];  
        for (int j = 0; j < BOX_SIZE; j++) {
            int c = boxCol + j;
            if ((r != row || c != col) && boxRowCells[c].values[value]) {
                boxRowCells[c].values[value] = false;
                boxRowCells[c].count--;
            }
        }
    }
    
    return true;  
}

void init_constraints(int map[SIZE][SIZE], ConstraintSet constraints) {
    for (int i = 0; i < SIZE; i++) {
        
        // МНЗ Кэш указателя на строку
        // for (int j = 0; j < SIZE; j++) {
        //     constraints[i][j].count = 0;
        //     for (int val = 0; val <= SIZE; val++) {
        //         constraints[i][j].values[val] = false;
        //     }
            
        //     if (map[i][j] == 0) {
        //         constraints[i][j].count = SIZE;
        //         for (int val = 1; val <= SIZE; val++) {
        //             constraints[i][j].values[val] = true;
        //         }
        //     }
        // }

        CandidateSet* rowCells = constraints[i];

        for (int j = 0; j < SIZE; j++) {
            CandidateSet* cell = &rowCells[j];
            cell->count = 0;
            for (int val = 0; val <= SIZE; val++) {
                cell->values[val] = false;
            }
            
            if (map[i][j] == 0) {
                cell->count = SIZE;
                for (int val = 1; val <= SIZE; val++) {
                    cell->values[val] = true;
                }
            }
        }
    }
    
    for (int i = 0; i < SIZE; i++) {

        // МНЗ Объединяем повторяющийся доступ к constraints[i]
        // for (int j = 0; j < SIZE; j++) {
        //     int val = map[i][j];
        //     if (val != 0) {
        //         update_constraints(constraints, i, j, val);
        //     }
        // }
        for (int i = 0; i < SIZE; i++) {
            CandidateSet* rowCells = constraints[i];
            for (int j = 0; j < SIZE; j++) {
                int val = map[i][j];
                if (val != 0) {
                    update_constraints(constraints, i, j, val);
                }
            }
        }
    }
}

Cell find_most_constrained_cell(int map[SIZE][SIZE], ConstraintSet constraints) {
    Cell result = {-1, -1};
    int minCount = SIZE + 1;
    
    for (int i = 0; i < SIZE; i++) {

        // МНЗ Вместо повторных обращений к constraints[i]
        // for (int j = 0; j < SIZE; j++) {
        //     if (constraints[i][j].count > 0 && 
        //     constraints[i][j].count < minCount && 
        //     map[i][j] == 0) {
        //         minCount = constraints[i][j].count;
        //         result.row = i;
        //         result.col = j;
        //     }
        // }
        CandidateSet* rowCells = constraints[i]; 
        for (int j = 0; j < SIZE; j++) {
            CandidateSet* cell = &rowCells[j];
            if (cell->count > 0 && 
                cell->count < minCount && 
                map[i][j] == 0) {
                minCount = cell->count;
                result.row = i;
                result.col = j;
            }
        }

    }
    
    return result;
}

static bool solve_constraint_rec(int map[SIZE][SIZE], time_t start_time) {
    if (difftime(time(NULL), start_time) > TIME_LIMIT) {
        return false;
    }

    ConstraintSet constraints;
    init_constraints(map, constraints);
    
    Cell cell = find_most_constrained_cell(map, constraints);
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
        if (constraints[cell.row][cell.col].values[val]) {
            memcpy(temp, constraints, sizeof(ConstraintSet));
            
            map[cell.row][cell.col] = val;
            if (update_constraints(constraints, cell.row, cell.col, val)) {
                if (solve_constraint_rec(map, start_time)) {
                    return true;
                }
            }
            
            map[cell.row][cell.col] = 0;
            memcpy(constraints, temp, sizeof(ConstraintSet));
        }
    }
    
    return false;
}

bool solve_constraint(int map[SIZE][SIZE]) {
    time_t start_time = time(NULL);
    return solve_constraint_rec(map, start_time);
}

////////////////////////////////////////////////////////////////

static inline int get_box_index_16(int row, int col) {
    return (row / 4) * 4 + (col / 4);
}

void init_constraints_16(int map[SIZE][SIZE], ConstraintSet16* constraints) {
    memset(constraints, 0, sizeof(ConstraintSet16));
    
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
                update_constraints_16(constraints, i, j, val);
            }
        }
    }
}

bool update_constraints_16(ConstraintSet16* constraints, int row, int col, int value) {
    constraints->row[row][value] = true;
    constraints->col[col][value] = true;
    constraints->box[get_box_index_16(row, col)][value] = true;

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

    int boxRow = (row / 4) * 4;
    int boxCol = (col / 4) * 4;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
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

Cell find_most_constrained_cell_16(int map[SIZE][SIZE], ConstraintSet16* constraints) {
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

static bool solve_constraint_16_rec(int map[SIZE][SIZE], time_t start_time) {
    if (difftime(time(NULL), start_time) > TIME_LIMIT) {
        return false;
    }

    ConstraintSet16 constraints;
    init_constraints_16(map, &constraints);

    Cell cell = find_most_constrained_cell_16(map, &constraints);
    if (cell.row == -1) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (map[i][j] == 0) return false;
            }
        }
        return true;
    }

    ConstraintSet16 temp;

    for (int val = 1; val <= SIZE; val++) {
        if (constraints.candidates[cell.row][cell.col].values[val]) {
            memcpy(&temp, &constraints, sizeof(ConstraintSet16));

            map[cell.row][cell.col] = val;
            if (update_constraints_16(&constraints, cell.row, cell.col, val)) {
                if (solve_constraint_16_rec(map, start_time)) {
                    return true;
                }
            }

            map[cell.row][cell.col] = 0;
            memcpy(&constraints, &temp, sizeof(ConstraintSet16));
        }
    }

    return false;
}

bool solve_constraint_16(int map[SIZE][SIZE]) {
    time_t start_time = time(NULL);
    return solve_constraint_16_rec(map, start_time);
}
