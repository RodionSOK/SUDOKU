#include <stdio.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>     
#include "include/sudoku_solver.h"
#include "include/benchmark.h"

///КОНСТАНТЫ

int X = 1, Y = 1;
int WHEIGHT = 500;
int WWIDTH = 500;
int map[SIZE][SIZE] = {0};

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

float abs_float(float number) {
    if (number < 0) {
        number *= -1;
    }
    return number;
}

///ОБРАБОТКА КНОПОК

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        if (solve_sudoku(map, SOLVER_MRV)) {
            printf("Решение найдено:\n");
            print_map(map);
        } else {
            printf("Решение не найдено.\n");
        }
    }
    
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        run_benchmark();
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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
        clear_map(map);
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
    if ((a != 1) && (a != 7)) Line(0.3, 0.5, 0.7, 0.5);
    if ((a != 1) && (a != 4) && (a != 7)) Line(0.3, 0.15, 0.7, 0.15);

    if ((a != 5) && (a !=6)) Line(0.7, 0.5, 0.7, 0.85);
    if ((a != 2)) Line(0.7, 0.5, 0.7, 0.15);

    if ((a != 1) && (a != 2) && (a != 3) && (a != 7)) Line(0.3, 0.5, 0.3, 0.85);
    if ((a == 2) || (a == 6) || (a == 8)) Line(0.3, 0.5, 0.3, 0.15);
    
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
        glfwGetCursorPos(window, &xpos, &ypos); 
        
        X = xpos / WWIDTH * 11;
        Y = abs_float( (ypos - WHEIGHT) / WHEIGHT * 11);
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
                if (InMap()) {
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

    window = glfwCreateWindow(WWIDTH, WHEIGHT, "SUDOKU", NULL, NULL);
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

