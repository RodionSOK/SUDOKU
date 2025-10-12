CC = gcc
CFLAGS = -Wall -std=c99 -I/usr/local/include -I/opt/homebrew/include
LIBS = -L/usr/local/lib -L/opt/homebrew/lib -lglfw -framework OpenGL

SOURCES = main.c \
    src/sudoku_solver.c \
    src/benchmark.c \
    src/test_puzzles.c \
    src/solver_backtrack.c \
    src/solver_mrv.c \
    src/solver_dl.c \
    src/solver_constraint.c

TARGET = sudoku

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -Iinclude $(SOURCES) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

rebuild: clean all

.PHONY: all run clean rebuild