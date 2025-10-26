#include "solver_mrv.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

// МНЗ Код функции был встроен в места использования
// static bool is_valid_move(int map[SIZE][SIZE], int row, int col, int num) {
//     for (int j = 0; j < SIZE; j++) {
//         if (map[row][j] == num) {
//             return false;
//         }
//     }
    
//     for (int i = 0; i < SIZE; i++) {
//         if (map[i][col] == num) {
//             return false;
//         }
//     }
    
//     int start_row = (row / 3) * 3;
//     int start_col = (col / 3) * 3;
//     for (int i = start_row; i < start_row + 3; i++) {
//         for (int j = start_col; j < start_col + 3; j++) {
//             if (map[i][j] == num) {
//                 return false;
//             }
//         }
//     }
    
//     return true;
// }


static int count_possibilities(int map[SIZE][SIZE], int row, int col) {
    if (map[row][col] != 0) return 0;

    // МНЗ Вынос инварианта start_row + 3, end_row + 3
    // for (int i = start_row; i < start_row + 3; i++)

    int start_row = (row / BOX_SIZE) * BOX_SIZE;
    int start_col = (col / BOX_SIZE) * BOX_SIZE;
    int end_row = start_row + BOX_SIZE;
    int end_col = start_col + BOX_SIZE;

    int used_mask = 0;
    

    // МНЗ Использование битовой маски вместо операций сравнения
    // for (int j = 0; j < SIZE; j++) {
    //     if (map[row][j] == num) {
    //         return false;
    //     }
    // }
    
    // МНЗ объединение двух циклов в один
    // for (int j = 0; j < SIZE; j++) {
    //     if (map[row][j] != 0) {
    //         used_mask |= (1 << map[row][j]);
    //     }
    // }
    
    // for (int i = 0; i < SIZE; i++) {
    //     if (map[i][col] != 0) {
    //         used_mask |= (1 << map[i][col]);
    //     }
    // }

    for (int k = 0; k < SIZE; k++) {
        if (map[row][k] != 0) used_mask |= (1 << map[row][k]);
        if (map[k][col] != 0) used_mask |= (1 << map[k][col]);
    }
    
    // МНЗ Заменена вложенная итерация по сетке на одномерную
    // for (int i = start_row; i < end_row; i++) {
    //     for (int j = start_col; j < end_col; j++) {
    //         if (map[i][j] != 0) {
    //             used_mask |= (1 << map[i][j]);
    //         }
    //     }
    // }

    for (int k = 0; k < BOX_SIZE * BOX_SIZE; k++) {
        int i = start_row + (k / BOX_SIZE);
        int j = start_col + (k % BOX_SIZE);
        if (map[i][j] != 0) used_mask |= (1 << map[i][j]);
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
    
    // МНЗ Заменена вложенная итерация по сетке на одномерную
    // for (int i = 0; i < SIZE; i++) {
    //     for (int j = 0; j < SIZE; j++) {
    for (int k = 0; k < SIZE * SIZE; k++) {
        int i = k / SIZE;
        int j = k % SIZE;
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
    
    if (!found_empty) {
        return 0;
    }
    
    return found_valid ? 1 : -1; 
}

static bool solve_mrv_recursive(int map[SIZE][SIZE], time_t start_time) {
    if (difftime(time(NULL), start_time) > TIME_LIMIT) {
        return false;
    }

    int row, col;
    int result = find_mrv_cell(map, &row, &col);
    
    if (result == 0) return true; 
    if (result == -1) return false;
    
    int start_row = (row / BOX_SIZE) * BOX_SIZE;
    int start_col = (col / BOX_SIZE) * BOX_SIZE;
    int end_row = start_row + BOX_SIZE;
    int end_col = start_col + BOX_SIZE;
    
    int used_mask = 0;
    
    // МНЗ объединение двух циклов в один
    // for (int j = 0; j < SIZE; j++) {
    //     if (map[row][j] != 0) {
    //         used_mask |= (1 << map[row][j]);
    //     }
    // }
    
    // for (int i = 0; i < SIZE; i++) {
    //     if (map[i][col] != 0) {
    //         used_mask |= (1 << map[i][col]);
    //     }
    // }

    for (int k = 0; k < SIZE; k++) {
        if (map[row][k] != 0) used_mask |= (1 << map[row][k]);
        if (map[k][col] != 0) used_mask |= (1 << map[k][col]);
    }
    
    // МНЗ Заменена вложенная итерация по сетке на одномерную
    // for (int i = start_row; i < end_row; i++) {
    //     for (int j = start_col; j < end_col; j++) {
    //         if (map[i][j] != 0) {
    //             used_mask |= (1 << map[i][j]);
    //         }
    //     }
    // }

    for (int k = 0; k < BOX_SIZE * BOX_SIZE; k++) {
        int i = start_row + (k / BOX_SIZE);
        int j = start_col + (k % BOX_SIZE);
        if (map[i][j] != 0) used_mask |= (1 << map[i][j]);
    }
    
    for (int num = 1; num <= SIZE; num++) {
        if (!(used_mask & (1 << num))) {
            map[row][col] = num;
            
            if (solve_mrv_recursive(map, start_time)) {
                return true;
            }
            
            map[row][col] = 0;
        }
    }
    
    return false;
}

bool solve_mrv(int map[SIZE][SIZE]) {
    time_t start_time = time(NULL);
    return solve_mrv_recursive(map, start_time);
}


////////////////////////////////////////////////////////////////////////////////////////

static bool is_valid_move_16(int map[SIZE][SIZE], int row, int col, int num) {
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
    
    int start_row = (row / 4) * 4;
    int start_col = (col / 4) * 4;
    for (int i = start_row; i < start_row + 4; i++) {
        for (int j = start_col; j < start_col + 4; j++) {
            if (map[i][j] == num) {
                return false;
            }
        }
    }
    
    return true;
}

static int count_possibilities_16(int map[SIZE][SIZE], int row, int col) {
    if (map[row][col] != 0) {
        return 0; 
    }
    
    int count = 0;
    for (int num = 1; num <= SIZE; num++) {
        if (is_valid_move_16(map, row, col, num)) {
            count++;
        }
    }
    return count;
}

static int find_mrv_cell_16(int map[SIZE][SIZE], int *best_row, int *best_col) {
    int min_possibilities = SIZE + 1; 
    bool found_empty = false;
    bool found_valid = false;
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == 0) { 
                found_empty = true;
                int possibilities = count_possibilities_16(map, i, j);
                
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

static bool solve_mrv_recursive_16(int map[SIZE][SIZE], time_t start_time) {
    if (difftime(time(NULL), start_time) > TIME_LIMIT) {
        return false;
    }

    int row, col;
    int result = find_mrv_cell_16(map, &row, &col);

    if (result == 0) {
        return true;
    }

    if (result == -1) {
        return false;
    }

    for (int num = 1; num <= SIZE; num++) {
        if (is_valid_move_16(map, row, col, num)) {
            map[row][col] = num;
            if (solve_mrv_recursive_16(map, start_time)) {
                return true;
            }
            map[row][col] = 0;
        }
    }

    return false;
}

bool solve_mrv_16(int map[SIZE][SIZE]) {
    time_t start_time = time(NULL);
    return solve_mrv_recursive_16(map, start_time);
}
