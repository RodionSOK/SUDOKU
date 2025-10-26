#include "sudoku_solver.h"
#include "solver_backtrack.h"
#include "solver_mrv.h"
#include "solver_dl.h"
#include "solver_constraint.h"
#include <stdio.h>
#include <string.h>


bool solve_sudoku(int map[SIZE][SIZE], SolverType type) {
    bool solved = false;

    switch (type) {
        case SOLVER_BACKTRACK:
            solved = solve_backtrack(map);
            break;
        case SOLVER_BACKTRACK_16:
            solved = solve_backtrack_16(map);
            break;
        case SOLVER_MRV:
            solved = solve_mrv(map);            
            break;
        // case SOLVER_MRV_16:
        //     solved = solve_mrv_16(map);
        //     break;
        case SOLVER_DLX:
            solved = solve_dl(map);
            break;
        // case SOLVER_DLX_16:
        //     solved = solve_dl_16(map);
        //     break;
        case SOLVER_CONSTRAINT:
            solved = solve_constraint(map);
            break;
        // case SOLVER_CONSTRAINT_16:
        //     solved = solve_constraint_16(map);
        //     break;
        default:
            printf("Неизвестный тип решателя\n");
            return false;
    }

    return solved;
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