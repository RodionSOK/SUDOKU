#include "solver_dl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ROWS SIZE * SIZE * SIZE
#define COLS SIZE * SIZE * 4

static DLNode header;
static ColumnHeader columns[COLS];
static DLNode nodes[ROWS * 4]; 
static int solution[SIZE * SIZE];
static int solution_count;
static int node_count;

static void init_dlx() {
    node_count = 0;
    solution_count = 0;
    
    header.left = header.right = &header;
    header.up = header.down = &header;
    header.column = &header;
    
    for (int i = 0; i < COLS; i++) {
        columns[i].node.up = columns[i].node.down = &columns[i].node;
        columns[i].node.column = &columns[i].node;
        columns[i].size = 0;
        
        columns[i].node.left = header.left;
        columns[i].node.right = &header;
        header.left->right = &columns[i].node;
        header.left = &columns[i].node;
    }
}

static void add_node(int row_id, int col) {
    DLNode* node = &nodes[node_count++];
    ColumnHeader* column = &columns[col];
    
    node->row = row_id;  
    node->col = col;
    node->column = &column->node;
    
    node->up = column->node.up;
    node->down = &column->node;
    column->node.up->down = node;
    column->node.up = node;
    
    column->size++;
}

static void cover(ColumnHeader* col) {
    col->node.right->left = col->node.left;
    col->node.left->right = col->node.right;
    
    for (DLNode* row = col->node.down; row != &col->node; row = row->down) {
        for (DLNode* node = row->right; node != row; node = node->right) {
            node->up->down = node->down;
            node->down->up = node->up;
            ((ColumnHeader*)node->column)->size--;
        }
    }
}

static void uncover(ColumnHeader* col) {
    for (DLNode* row = col->node.up; row != &col->node; row = row->up) {
        for (DLNode* node = row->left; node != row; node = node->left) {
            ((ColumnHeader*)node->column)->size++;
            node->up->down = node;
            node->down->up = node;
        }
    }
    
    col->node.right->left = &col->node;
    col->node.left->right = &col->node;
}

static ColumnHeader* choose_column() {
    ColumnHeader* best = NULL;
    int min_size = ROWS + 1;
    

    // МНЗ Добавление условия выхода
    // for (DLNode* col = header.right; col != &header; col = col->right) {
    //         ColumnHeader* column = (ColumnHeader*)col;
    //         if (column->size < min_size) {
    //             min_size = column->size;
    //             best = column;
    //         }
    //     }


    for (DLNode* col = header.right; col != &header; col = col->right) {
        ColumnHeader* column = (ColumnHeader*)col;
        
        if (column->size == 0) {
            return column;  
        }
        
        if (column->size < min_size) {
            min_size = column->size;
            best = column;
            
            if (min_size == 1) {
                return best;  
            }
        }
    }
    
    return best;
}

static bool search(int depth) {
    if (header.right == &header) {
        solution_count = depth;
        return true; 
    }
    
    ColumnHeader* col = choose_column();
    if (col->size == 0) {
        return false;
    }
    
    cover(col);
    
    for (DLNode* row = col->node.down; row != &col->node; row = row->down) {
        solution[depth] = row->row;
        
        for (DLNode* node = row->right; node != row; node = node->right) {
            cover((ColumnHeader*)node->column);
        }
        
        if (search(depth + 1)) {
            return true;  
        }
        
        for (DLNode* node = row->left; node != row; node = node->left) {
            uncover((ColumnHeader*)node->column);
        }
    }
    
    uncover(col);
    return false;
}

static void build_matrix(int map[SIZE][SIZE]) {
    init_dlx();
    
    int row_id = 0;
    
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            for (int v = 1; v <= SIZE; v++) {
                if (map[r][c] != 0 && map[r][c] != v) {
                    row_id++;
                    continue;
                }
                
                add_node(row_id, r * SIZE + c);
                
                add_node(row_id, SIZE * SIZE + r * SIZE + (v - 1));
                
                add_node(row_id, 2 * SIZE * SIZE + c * SIZE + (v - 1));
                
                int box = (r / BOX_SIZE) * BOX_SIZE + (c / BOX_SIZE);
                add_node(row_id, 3 * SIZE * SIZE + box * SIZE + (v - 1));
                
                for (int i = 0; i < 4; i++) {
                    int idx = node_count - 4 + i;
                    if (i == 0) {
                        nodes[idx].left = &nodes[node_count - 1];
                        nodes[idx].right = &nodes[idx + 1];
                    } else if (i == 3) {
                        nodes[idx].left = &nodes[idx - 1];
                        nodes[idx].right = &nodes[node_count - 4];
                    } else {
                        nodes[idx].left = &nodes[idx - 1];
                        nodes[idx].right = &nodes[idx + 1];
                    }
                }
                
                row_id++;
            }
        }
    }
}

static void extract_solution(int map[SIZE][SIZE]) {
    for (int i = 0; i < solution_count; i++) {
        int row_id = solution[i];
       
        int r = row_id / (SIZE * SIZE);      
        int c = (row_id / SIZE) % SIZE;      
        int v = (row_id % SIZE) + 1;         

        // МНЗ Удаление избыточного условия
        // if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && v >= 1 && v <= SIZE) {

        map[r][c] = v;
    }
}

bool solve_dl(int map[SIZE][SIZE]) {
    build_matrix(map);
    
    if (search(0)) {
        extract_solution(map);
        return true;
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////

#define ROWS_16 4096
#define COLS_16 1024

static DLNode header_16;
static ColumnHeader columns_16[COLS_16];
static DLNode nodes_16[ROWS_16 * 4]; 
static int solution_16[256];
static int solution_count_16;
static int node_count_16;

static void init_dlx_16() {
    node_count_16 = 0;
    solution_count_16 = 0;
    
    header_16.left = header_16.right = &header_16;
    header_16.up = header_16.down = &header_16;
    header_16.column = &header_16;
    
    for (int i = 0; i < COLS_16; i++) {
        columns_16[i].node.up = columns_16[i].node.down = &columns_16[i].node;
        columns_16[i].node.column = &columns_16[i].node;
        columns_16[i].size = 0;
        
        columns_16[i].node.left = header_16.left;
        columns_16[i].node.right = &header_16;
        header_16.left->right = &columns_16[i].node;
        header_16.left = &columns_16[i].node;
    }
}

static void add_node_16(int row_id, int col) {
    DLNode* node = &nodes_16[node_count_16++];
    ColumnHeader* column = &columns_16[col];
    
    node->row = row_id;  // Сохраняем row_id, а не индекс строки
    node->col = col;
    node->column = &column->node;
    
    node->up = column->node.up;
    node->down = &column->node;
    column->node.up->down = node;
    column->node.up = node;
    
    column->size++;
}

static void cover_16(ColumnHeader* col) {
    col->node.right->left = col->node.left;
    col->node.left->right = col->node.right;
    
    for (DLNode* row = col->node.down; row != &col->node; row = row->down) {
        for (DLNode* node = row->right; node != row; node = node->right) {
            node->up->down = node->down;
            node->down->up = node->up;
            ((ColumnHeader*)node->column)->size--;
        }
    }
}

static void uncover_16(ColumnHeader* col) {
    for (DLNode* row = col->node.up; row != &col->node; row = row->up) {
        for (DLNode* node = row->left; node != row; node = node->left) {
            ((ColumnHeader*)node->column)->size++;
            node->up->down = node;
            node->down->up = node;
        }
    }
    
    col->node.right->left = &col->node;
    col->node.left->right = &col->node;
}

static ColumnHeader* choose_column_16() {
    ColumnHeader* best = NULL;
    int min_size = ROWS_16 + 1;

    for (DLNode* col = header_16.right; col != &header_16; col = col->right) {
        ColumnHeader* column = (ColumnHeader*)col;
        if (column->size < min_size) {
            min_size = column->size;
            best = column;
        }
    }

    return best;
}

static bool search_16(int depth) {
    if (header_16.right == &header_16) {
        solution_count_16 = depth;
        return true;
    }

    ColumnHeader* col = choose_column_16();
    if (col->size == 0) {
        return false;
    }

    cover_16(col);

    for (DLNode* row = col->node.down; row != &col->node; row = row->down) {
        solution_16[depth] = row->row;

        for (DLNode* node = row->right; node != row; node = node->right) {
            cover_16((ColumnHeader*)node->column);
        }

        if (search_16(depth + 1)) {
            return true;
        }

        for (DLNode* node = row->left; node != row; node = node->left) {
            uncover_16((ColumnHeader*)node->column);
        }
    }

    uncover_16(col);
    return false;
}

static void build_matrix_16(int map[SIZE][SIZE]) {
    init_dlx_16();

    int row_id = 0;

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            for (int v = 1; v <= SIZE; v++) {
                if (map[r][c] != 0 && map[r][c] != v) {
                    row_id++;
                    continue;
                }

                add_node_16(row_id, r * SIZE + c);
                add_node_16(row_id, 256 + r * SIZE + (v - 1));
                add_node_16(row_id, 512 + c * SIZE + (v - 1));
                int box = (r / 4) * 4 + (c / 4);
                add_node_16(row_id, 768 + box * SIZE + (v - 1));

                for (int i = 0; i < 4; i++) {
                    int idx = node_count_16 - 4 + i;
                    if (i == 0) {
                        nodes_16[idx].left = &nodes_16[node_count_16 - 1];
                        nodes_16[idx].right = &nodes_16[idx + 1];
                    } else if (i == 3) {
                        nodes_16[idx].left = &nodes_16[idx - 1];
                        nodes_16[idx].right = &nodes_16[node_count_16 - 4];
                    } else {
                        nodes_16[idx].left = &nodes_16[idx - 1];
                        nodes_16[idx].right = &nodes_16[idx + 1];
                    }
                }

                row_id++;
            }
        }
    }
}

static void extract_solution_16(int map[SIZE][SIZE]) {
    for (int i = 0; i < solution_count_16; i++) {
        int row_id = solution_16[i];
        int r = row_id / (SIZE * SIZE);
        int c = (row_id / SIZE) % SIZE;
        int v = (row_id % SIZE) + 1;
        map[r][c] = v;
    }
}

bool solve_dl_16(int map[SIZE][SIZE]) {
    build_matrix_16(map);
    
    if (search_16(0)) {
        extract_solution_16(map);
        return true;
    }
    
    return false;
}
