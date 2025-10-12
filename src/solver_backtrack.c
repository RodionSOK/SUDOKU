#include "solver_backtrack.h"
#include <stdbool.h>

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
        bool vGrid = false;

        for (int val = map[row][col] + 1; val <= SIZE && !vGrid; val++) {
            map[row][col] = val;
            vGrid = validGrid(map, row, col);
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