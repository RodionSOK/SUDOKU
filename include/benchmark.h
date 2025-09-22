#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "sudoku_solver.h"
#include <stdbool.h>

typedef struct {
    double time_taken;
    int iterations;
    bool solved;
} BenchmarkResult;

double get_time_microseconds();
BenchmarkResult benchmark_single_puzzle(int puzzle[SIZE][SIZE], const char* name);
void run_benchmark();
void print_benchmark_results(BenchmarkResult results[], const char* names[], int count);

#endif
