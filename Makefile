CC = gcc
CFLAGS = -Wall -std=c99 -I/usr/local/include -I/opt/homebrew/include
LIBS = -L/usr/local/lib -L/opt/homebrew/lib -lglfw -framework OpenGL

# Исходные файлы
SOURCES = main.c src/sudoku_solver.c src/benchmark.c src/test_puzzles.c
TARGET = sudoku

# Основная цель
all: $(TARGET)

# Сборка программы
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -Iinclude $(SOURCES) -o $(TARGET) $(LIBS)

# Запуск
run: $(TARGET)
	./$(TARGET)

# Очистка
clean:
	rm -f $(TARGET)

# Пересборка
rebuild: clean all

.PHONY: all run clean rebuild