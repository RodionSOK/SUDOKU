#include <stdio.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>

///КОНСТАНТЫ

#define SIZE 9

int X = 1, Y = 1;

int WHEIGHT = 500;
int WWIDTH = 500;

int map[SIZE][SIZE] = {0};

///РЕШЕНИЕ ТАБЛИЦЫ

typedef struct {
    int row;
    int col;
} Cell;

typedef struct {
    Cell emptyCells[SIZE * SIZE];
    int emptyCount;
    int currentEmptyIndex;
} Solver;

bool validGrid(Solver* solver, int row, int col) {
    int value = map[row][col];

    // Проверка строки
    for (int i = 0; i < SIZE; i++) {
        if (i != col && map[row][i] == value)
            return false;
    }

    // Проверка столбца
    for (int i = 0; i < SIZE; i++) {
        if (i != row && map[i][col] == value)
            return false;
    }

    // Проверка блока 3x3
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if ((i != row || j != col) && map[i][j] == value)
                return false;
        }
    }

    return true;
}

bool solveBT(Solver* solver) {
    if (solver->emptyCount == 0) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (map[i][j] == 0) {
                    solver->emptyCells[solver->emptyCount].row = i;
                    solver->emptyCells[solver->emptyCount].col = j;
                    solver->emptyCount++;
                }
            }
        }
        solver->currentEmptyIndex = 0;
    }

    while (solver->currentEmptyIndex < solver->emptyCount) {
        Cell currentCell = solver->emptyCells[solver->currentEmptyIndex];
        int row = currentCell.row;
        int col = currentCell.col;
        bool vGrid = false;

        for (int val = map[row][col] + 1; val <= SIZE && !vGrid; val++) {
            map[row][col] = val;
            vGrid = validGrid(solver, row, col);
        }

        if (vGrid) {
            solver->currentEmptyIndex++;
        } else {
            if (solver->currentEmptyIndex == 0) {
                return false;
            } else {
                map[row][col] = 0;
                solver->currentEmptyIndex--;
            }
        }
    }
    return true;
}

///ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ

float rgb(float x)
{
    return x / 255.0;
}

int InMap() {
    if (1 <= X && X <= 10 && 1 <= Y && Y<= 10) {

        return 1;
    }
    return 0;
}

float abs(float number) {
    if (number < 0) {
        number *= -1;
    }
    return number;
}

///ОБРАБОТКА КНОПОК

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        
        Solver solver = {
            .emptyCount = 0,
            .currentEmptyIndex = 0
        };

        if (solveBT(&solver)) {
            printf("Решение найдено:\n");
            for (int i = SIZE - 1; i >= 0; i--) {
                for (int j = 0; j < SIZE; j++) {
                    printf("%d ", map[j][i]);
                }
                printf("\n");
            }
        } else {
            printf("Решение не найдено.\n");
        }
    }
    
    if (GLFW_KEY_0 <= key && key <= GLFW_KEY_9 && GLFW_PRESS) {
        int number = key - '0';
        
        if (InMap()) {
            map[X - 1][Y - 1] = number;
        }
    }
    
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        if (X < 9) {
            X += 1;
        }
    };
    
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        if (X > 1) {
            X -= 1;
        }
    };
    
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        if (Y < 9) {
            Y += 1;
        }
    };
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if (Y > 1) {
            Y -= 1;
        }
    }
    
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                map[i][j] = 0;
            }
        }
    }
}

///ПОСТРОЕНИЕ ТАБЛИЦЫ

void Line(float x1, float y1, float x2, float y2)
{
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
}

void ShowCount(int a)
{
    glLineWidth(5);
    glColor3f(rgb(166),rgb(36),rgb(36));
    glBegin(GL_LINES);
    
    if ((a != 1) && (a != 4)) Line(0.3, 0.85, 0.7, 0.85);
    if ((a != 0) && (a != 1) && (a != 7)) Line(0.3, 0.5, 0.7, 0.5);
    if ((a != 1) && (a != 4) && (a != 7)) Line(0.3, 0.15, 0.7, 0.15);

    if ((a != 5) && (a !=6)) Line(0.7, 0.5, 0.7, 0.85);
    if ((a != 2)) Line(0.7, 0.5, 0.7, 0.15);

    if ((a != 1) && (a != 2) && (a != 3) && (a != 7)) Line(0.3, 0.5, 0.3, 0.85);
    if ((a == 0) || (a == 2) || (a == 6) || (a == 8)) Line(0.3, 0.5, 0.3, 0.15);
    
    glEnd();
}

void Hightlighting() {
    float dots[] = {
        0.02, 0.02,
        0.02, 0.98,
        0.98, 0.98,
        0.98, 0.02
    };
    
    float colors[] = {
        rgb(255),rgb(136),rgb(136),  rgb(255),rgb(136),rgb(136),  rgb(255),rgb(136),rgb(136),  rgb(255),rgb(136),rgb(136),
    };
    
    glVertexPointer(2, GL_FLOAT, 0, dots);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, colors);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos); // Получаем координаты курсора
        
        X = xpos / WWIDTH * 11;
        Y = abs( (ypos - WHEIGHT) / WHEIGHT * 11);
    }
}

void ShowField()
{
    float dots[] = {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        
        0.02, 0.02,
        0.02, 0.98,
        0.98, 0.98,
        0.98, 0.02
    };
    float colors[] = {
        rgb(255),rgb(136),rgb(136),  rgb(255),rgb(136),rgb(136),  rgb(255),rgb(136),rgb(136),  rgb(255),rgb(136),rgb(136),
        rgb(255),rgb(189),rgb(189),  rgb(255),rgb(189),rgb(189),  rgb(255),rgb(189),rgb(189),  rgb(255),rgb(189),rgb(189),};

    glVertexPointer(2, GL_FLOAT, 0, dots);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, colors);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void ShowOutline()
{
    float outline[] = {
        -4.5 / 5.5, -4.5 / 5.5,
        -4.5 / 5.5, 4.5 / 5.5,
        4.5 / 5.5, 4.5 / 5.5,
        4.5 / 5.5, -4.5 / 5.5,
        
        -1.5 / 5.5, -4.5 / 5.5,
        -1.5 / 5.5, 4.5 / 5.5,
        1.5 / 5.5, -4.5 / 5.5,
        1.5 / 5.5, 4.5 / 5.5,
        
        -4.5 / 5.5, 1.5 / 5.5,
        4.5 / 5.5, 1.5 / 5.5,
        -4.5 / 5.5, -1.5 / 5.5,
        4.5 / 5.5, -1.5 / 5.5
    };
    
    float colors[] = {
        rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),
        rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),
        rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),  rgb(166),rgb(36),rgb(36),};

    glVertexPointer(2, GL_FLOAT, 0, outline);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, colors);
    glEnableClientState(GL_COLOR_ARRAY);

    glLineWidth(5);
    
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINES, 4, 4);
    glDrawArrays(GL_LINES, 8, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void ShowTable()
{
    glLoadIdentity();
    glPushMatrix();
    glScalef(2.0 / 11, 2.0 / 11, 1);
    glTranslatef(-9 * 0.5, -9 * 0.5, 0);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            glPushMatrix();
            glTranslatef(i, j, 0);

            ShowField();
            if (i == X - 1 && j == Y - 1) {
                if (InMap(X, Y)) {
                    Hightlighting();
                }
            }
            if (map[i][j] != 0) {
                ShowCount(map[i][j]);
            }

            glPopMatrix();
        }
    }

    glPopMatrix();
    ShowOutline();
}

///MAIN

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WHEIGHT, WWIDTH, "SUDOKU", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, KeyCallback);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glClearColor(rgb(255), rgb(255), rgb(255), 1);
        
        ShowTable();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

