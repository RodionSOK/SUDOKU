# Sudoku Solver and Visualizer with GUI

## Description

This program is an interactive Sudoku puzzle tool featuring:
- Visual 9x9 Sudoku grid editor
- Backtracking algorithm solver
- Simple graphical interface using OpenGL/GLFW

## Features

- **Graphical Interface**:
  - Edit cells with mouse and keyboard
  - Active cell highlighting
  - Seven-segment style digit display
- **Solving Algorithm**:
  - Backtracking implementation
  - Validity checking for number placement
  - Standard Sudoku rules enforcement
- **Controls**:
  - Arrow keys for navigation
  - Digits 0-9 for input/erasure
  - Enter - solve current puzzle
  - R - reset the board

## Requirements

- C compiler with C99 support
- Libraries:
  - GLFW 3.x
  - OpenGL

## Build and Run

gcc sudoku.c -o sudoku -lglfw -lGL
./sudoku

## Usage

1. **Editing the Board**:
   - Click on a cell to select it
   - Press a digit (1-9) or 0 to clear
   - Use arrow keys to navigate

2. **Solving**:
   - Press Enter to run the solver
   - Solution appears in console and on the board

3. **Clearing**:
   - Press R to reset the entire board

## Program Architecture

### Core Components:
1. **Graphics Engine**:
   - Grid and digit rendering
   - Input handling (mouse/keyboard)

2. **Sudoku Logic**:
   - Data structures for board state
   - Validity checking
   - Backtracking solver

3. **Interface**:
   - Event callback functions
   - Visual feedback system

## Limitations

- Fixed window size (500x500)
- No save/load functionality
- Doesn't verify solution uniqueness
- Minimalist interface without menus

## Potential Enhancements

1. Add difficulty levels
2. Implement valid puzzle generator
3. Include timer and hint system
4. Improve graphical interface
