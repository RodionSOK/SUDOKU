#include "benchmark.h"
#include "test_puzzles.h"
#include "sudoku_solver.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

const char* get_solver_name(SolverType type) {
    switch (type) {
        case SOLVER_BACKTRACK:
            return "Backtracking";
        case SOLVER_BACKTRACK_16:
            return "Backtracking";
        case SOLVER_MRV:
            return "MRV Heuristic";
        // case SOLVER_MRV_16:
        //     return "MRV Heuristic 16x16";
        case SOLVER_DLX:
            return "Dancing Links";
        // case SOLVER_DLX_16:
        //     return "Dancing Links 16x16";
        case SOLVER_CONSTRAINT:
            return "Constraint";
        // case SOLVER_CONSTRAINT_16:
        //     return "Constraint 16x16";
        default:
            return "Unknown";
    }
}

double get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

BenchmarkResult benchmark_single_puzzle(int puzzle[SIZE][SIZE], SolverType type) {
    BenchmarkResult result = {0, false};
    int test_puzzle[SIZE][SIZE];
    
    memcpy(test_puzzle, puzzle, sizeof(int) * SIZE * SIZE);
    
    double start_time = get_time_microseconds();
    result.solved = solve_sudoku(test_puzzle, type);
    double end_time = get_time_microseconds();
    
    result.time_taken = end_time - start_time;
    return result;
}

void run_benchmark() {
    const char* difficulty_names[] = {"Easy", "Medium", "Hard", "Expert"};
    BenchmarkResult results[4];  
    const char* solver_names[4]; 
    
    int test_puzzles[4][SIZE][SIZE];
    SIZE == 9 ? get_test_puzzles(test_puzzles) : get_test_puzzles_16(test_puzzles);
    
    printf("\nBenchmark Results:\n");
    printf("========================================\n");
    
    for (int diff = 0; diff < 4; diff++) {
        printf("\nDifficulty: %s\n", difficulty_names[diff]);
        printf("----------------------------------------\n");
        printf("%-20s %-15s %-10s\n", "Solver", "Time (ms)", "Solved");
        printf("----------------------------------------\n");
        
        if (SIZE == 9) {
            results[0] = benchmark_single_puzzle(test_puzzles[diff], SOLVER_BACKTRACK);
            solver_names[0] = get_solver_name(SOLVER_BACKTRACK);
        }
        
        else if (SIZE == 16) {
            results[0] = benchmark_single_puzzle(test_puzzles[diff], SOLVER_BACKTRACK_16);
            solver_names[0] = get_solver_name(SOLVER_BACKTRACK_16);
        }

        results[1] = benchmark_single_puzzle(test_puzzles[diff], SOLVER_MRV);
        solver_names[1] = get_solver_name(SOLVER_MRV);

        results[2] = benchmark_single_puzzle(test_puzzles[diff], SOLVER_CONSTRAINT);
        solver_names[2] = get_solver_name(SOLVER_CONSTRAINT);

        results[3] = benchmark_single_puzzle(test_puzzles[diff], SOLVER_DLX);
        solver_names[3] = get_solver_name(SOLVER_DLX);

        for (int i = 0; i < 4; i++) {
            printf("%-20s %-15.3f %-10s\n",
                   solver_names[i],
                   results[i].time_taken / 1000.0,
                   results[i].solved ? "Yes" : "No");
        }
        printf("----------------------------------------\n");
    }
    printf("========================================\n");
}
