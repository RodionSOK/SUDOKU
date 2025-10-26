#include "solver_backtrack.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

bool solve_backtrack(int map[SIZE][SIZE]) {
    BacktrackSolver solver = {0}; 

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == 0) {
                solver.emptyCells[solver.emptyCount].row = i;
                solver.emptyCells[solver.emptyCount].col = j;
                solver.emptyCount++;
            }
        }
    }

    while (solver.currentEmptyIndex < solver.emptyCount) {
        Cell currentCell = solver.emptyCells[solver.currentEmptyIndex];
        int row = currentCell.row;
        int col = currentCell.col;


        // МНЗ Вынос инварианта startRow, startCol
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        bool vGrid = false;

        // МНЗ Инлай оптимизация, код функции был встроен в место использования
        //  for (int val = map[row][col] + 1; val <= SIZE && !vGrid; val++) {
        //     map[row][col] = val;
        //     vGrid = validGrid(map, row, col);
        // }


        for (int val = map[row][col] + 1; val <= SIZE && !vGrid; val++) {
            map[row][col] = val;
            
            bool isValid = true;
            
            // МНЗ Развертывание циклов для проверки строки, столбца
            // for (int i = 0; i < SIZE; i++) {
            //     if (i != col && map[row][i] == value)
            //         return false;
            // }

            if (isValid) {
                if ((0 != col && map[row][0] == val) ||
                    (1 != col && map[row][1] == val) ||
                    (2 != col && map[row][2] == val) ||
                    (3 != col && map[row][3] == val) ||
                    (4 != col && map[row][4] == val) ||
                    (5 != col && map[row][5] == val) ||
                    (6 != col && map[row][6] == val) ||
                    (7 != col && map[row][7] == val) ||
                    (8 != col && map[row][8] == val)) {
                    isValid = false;
                }
            }
            
            if (isValid) {
                if ((0 != row && map[0][col] == val) ||
                    (1 != row && map[1][col] == val) ||
                    (2 != row && map[2][col] == val) ||
                    (3 != row && map[3][col] == val) ||
                    (4 != row && map[4][col] == val) ||
                    (5 != row && map[5][col] == val) ||
                    (6 != row && map[6][col] == val) ||
                    (7 != row && map[7][col] == val) ||
                    (8 != row && map[8][col] == val)) {
                    isValid = false;
                }
            }
            
            if (isValid) {
                for (int i = startRow; i < startRow + 3 && isValid; i++) {
                    for (int j = startCol; j < startCol + 3 && isValid; j++) {
                        if ((i != row || j != col) && map[i][j] == val) {
                            isValid = false;
                        }
                    }
                }
            }
            
            vGrid = isValid;
        }


        if (vGrid) {
            solver.currentEmptyIndex++;
        } else {
            if (solver.currentEmptyIndex == 0) {
                return false;
            } else {
                map[row][col] = 0;
                solver.currentEmptyIndex--;
            }
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////


bool solve_backtrack_16(int map[SIZE][SIZE]) {
    BacktrackSolver solver = {0}; 
    time_t start_time = time(NULL);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == 0) {
                solver.emptyCells[solver.emptyCount].row = i;
                solver.emptyCells[solver.emptyCount].col = j;
                solver.emptyCount++;
            }
        }
    }

    while (solver.currentEmptyIndex < solver.emptyCount) {
        if (difftime(time(NULL), start_time) > TIME_LIMIT) {
            return false;
        }

        Cell currentCell = solver.emptyCells[solver.currentEmptyIndex];
        int row = currentCell.row;
        int col = currentCell.col;
        int startRow = (row / 4) * 4;
        int startCol = (col / 4) * 4;
        bool vGrid = false;

        for (int val = map[row][col] + 1; val <= SIZE && !vGrid; val++) {
            map[row][col] = val;
            bool isValid = true;

            if ((0 != col && map[row][0] == val) ||
                (1 != col && map[row][1] == val) ||
                (2 != col && map[row][2] == val) ||
                (3 != col && map[row][3] == val) ||
                (4 != col && map[row][4] == val) ||
                (5 != col && map[row][5] == val) ||
                (6 != col && map[row][6] == val) ||
                (7 != col && map[row][7] == val) ||
                (8 != col && map[row][8] == val) ||
                (9 != col && map[row][9] == val) ||
                (10 != col && map[row][10] == val) ||
                (11 != col && map[row][11] == val) ||
                (12 != col && map[row][12] == val) ||
                (13 != col && map[row][13] == val) ||
                (14 != col && map[row][14] == val) ||
                (15 != col && map[row][15] == val)) {
                isValid = false;
            }

            if (isValid) {
                if ((0 != row && map[0][col] == val) ||
                    (1 != row && map[1][col] == val) ||
                    (2 != row && map[2][col] == val) ||
                    (3 != row && map[3][col] == val) ||
                    (4 != row && map[4][col] == val) ||
                    (5 != row && map[5][col] == val) ||
                    (6 != row && map[6][col] == val) ||
                    (7 != row && map[7][col] == val) ||
                    (8 != row && map[8][col] == val) ||
                    (9 != row && map[9][col] == val) ||
                    (10 != row && map[10][col] == val) ||
                    (11 != row && map[11][col] == val) ||
                    (12 != row && map[12][col] == val) ||
                    (13 != row && map[13][col] == val) ||
                    (14 != row && map[14][col] == val) ||
                    (15 != row && map[15][col] == val)) {
                    isValid = false;
                }
            }

            if (isValid) {
                for (int i = startRow; i < startRow + 4 && isValid; i++) {
                    for (int j = startCol; j < startCol + 4 && isValid; j++) {
                        if ((i != row || j != col) && map[i][j] == val) {
                            isValid = false;
                        }
                    }
                }
            }

            vGrid = isValid;
        }

        if (vGrid) {
            solver.currentEmptyIndex++;
        } else {
            if (solver.currentEmptyIndex == 0) {
                return false;
            } else {
                map[row][col] = 0;
                solver.currentEmptyIndex--;
            }
        }
    }
    return true;
}
