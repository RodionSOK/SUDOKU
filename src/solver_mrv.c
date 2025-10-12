#include "solver_mrv.h"
#include <stdbool.h>

static bool is_valid_move(int map[SIZE][SIZE], int row, int col, int num) {
    for (int j = 0; j < SIZE; j++) {
        if (map[row][j] == num) {
            return false;
        }
    }
    
    for (int i = 0; i < SIZE; i++) {
        if (map[i][col] == num) {
            return false;
        }
    }
    
    int start_row = (row / 3) * 3;
    int start_col = (col / 3) * 3;
    for (int i = start_row; i < start_row + 3; i++) {
        for (int j = start_col; j < start_col + 3; j++) {
            if (map[i][j] == num) {
                return false;
            }
        }
    }
    
    return true;
}

static int count_possibilities(int map[SIZE][SIZE], int row, int col) {
    if (map[row][col] != 0) {
        return 0; 
    }
    
    int count = 0;
    for (int num = 1; num <= SIZE; num++) {
        if (is_valid_move(map, row, col, num)) {
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
    
    if (result == 0) {
        return true; 
    }
    
    if (result == -1) {
        return false; 
    }
    
    for (int num = 1; num <= SIZE; num++) {
        if (is_valid_move(map, row, col, num)) {
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
