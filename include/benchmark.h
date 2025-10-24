#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "sudoku_solver.h"
#include <stdbool.h>

typedef struct {
    double time_taken;
    bool solved;
} BenchmarkResult;

double get_time_microseconds();
BenchmarkResult benchmark_single_puzzle(int puzzle[SIZE][SIZE], SolverType type);
void run_benchmark();

#endif
