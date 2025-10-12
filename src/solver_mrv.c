#include "solver_mrv.h"
#include <stdbool.h>

static int count_possibilities(int map[SIZE][SIZE], int row, int col) {
    if (map[row][col] != 0) return 0;

    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;
    int end_row = start_row + 3;
    int end_col = start_col + 3;

    int used_mask = 0;
    
    for (int j = 0; j < SIZE; j++) {
        if (map[row][j] != 0) {
            used_mask |= (1 << map[row][j]);
        }
    }
    
    for (int i = 0; i < SIZE; i++) {
        if (map[i][col] != 0) {
            used_mask |= (1 << map[i][col]);
        }
    }
    
    for (int i = start_row; i < end_row; i++) {
        for (int j = start_col; j < end_col; j++) {
            if (map[i][j] != 0) {
                used_mask |= (1 << map[i][j]);
            }
        }
    }
    
    int count = 0;
    for (int num = 1; num <= SIZE; num++) {
        if (!(used_mask & (1 << num))) {
            count++;
        }
    }
    
    return count;
}

static int find_mrv_cell(int map[SIZE][SIZE], int *best_row, int *best_col) {
    int min_possibilities = SIZE + 1; 
    bool found_empty = false;
    bool found_valid = false;
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == 0) { 
                found_empty = true;
                int possibilities = count_possibilities(map, i, j);
                
                if (possibilities == 0) {
                    return -1; 
                }
                
                if (possibilities < min_possibilities) {
                    min_possibilities = possibilities;
                    *best_row = i;
                    *best_col = j;
                    found_valid = true;
                    
                    if (possibilities == 1) {
                        return 1; 
                    }
                }
            }
        }
    }
    
    if (!found_empty) {
        return 0;
    }
    
    return found_valid ? 1 : -1; 
}

static bool solve_mrv_recursive(int map[SIZE][SIZE]) {
    int row, col;
    int result = find_mrv_cell(map, &row, &col);
    
    if (result == 0) return true; 
    if (result == -1) return false;
    
    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;
    int end_row = start_row + 3;
    int end_col = start_col + 3;
    
    int used_mask = 0;
    
    for (int j = 0; j < SIZE; j++) {
        if (map[row][j] != 0) {
            used_mask |= (1 << map[row][j]);
        }
    }
    
    for (int i = 0; i < SIZE; i++) {
        if (map[i][col] != 0) {
            used_mask |= (1 << map[i][col]);
        }
    }
    
    for (int i = start_row; i < end_row; i++) {
        for (int j = start_col; j < end_col; j++) {
            if (map[i][j] != 0) {
                used_mask |= (1 << map[i][j]);
            }
        }
    }
    
    for (int num = 1; num <= SIZE; num++) {
        if (!(used_mask & (1 << num))) {
            map[row][col] = num;
            
            if (solve_mrv_recursive(map)) {
                return true;
            }
            
            map[row][col] = 0;
        }
    }
    
    return false;
}

bool solve_mrv(int map[SIZE][SIZE]) {
    return solve_mrv_recursive(map);
}
