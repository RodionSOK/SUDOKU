#include "../include/benchmark.h"
#include "../include/test_puzzles.h"
#include <stdio.h>
#include <sys/time.h>

double get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

BenchmarkResult benchmark_single_puzzle(int puzzle[SIZE][SIZE], const char* name) {
    BenchmarkResult result = {0, 0, false};
    int test_map[SIZE][SIZE];
    
    printf("Тестирование %s головоломки...\n", name);
    
    copy_map(puzzle, test_map);
    
    Solver solver = {
        .emptyCount = 0,
        .currentEmptyIndex = 0
    };
    
    double start_time = get_time_microseconds();
    result.solved = solveBT_benchmark(test_map, &solver, &result.iterations);
    double end_time = get_time_microseconds();
    
    result.time_taken = (end_time - start_time) / 1000.0; 
    
    if (result.solved) {
        printf("✓ Решено за %.2f мс (%d итераций)\n", 
               result.time_taken, result.iterations);
    } else {
        printf("✗ Не решено за %.2f мс (%d итераций)\n", 
               result.time_taken, result.iterations);
    }
    
    return result;
}


void run_benchmark() {
    printf("\n=== СУДОКУ БЕНЧМАРК ===\n");
    
    const int puzzle_count = 4;
    BenchmarkResult results[puzzle_count];
    const char* names[] = {"Простая", "Средняя", "Сложная", "Экспертная"};
    int puzzles[puzzle_count][SIZE][SIZE];
    
    copy_map(easy_puzzle, puzzles[0]);
    copy_map(medium_puzzle, puzzles[1]);
    copy_map(hard_puzzle, puzzles[2]);
    copy_map(expert_puzzle, puzzles[3]);
    
    for (int i = 0; i < puzzle_count; i++) {
        results[i] = benchmark_single_puzzle(puzzles[i], names[i]);
    }

    printf("\nНажмите СNTRL+C для завершения...\n\n");
}