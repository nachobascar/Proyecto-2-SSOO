#include "preparation.h"

void close_grid(char** grid) {
    for (int i = 0; i < 5; i++) {
        free(grid[i]);
    }
    free(grid);
}

void test_function() {
    char** grid = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        grid[i] = malloc(5 * sizeof(char));
        for (int j = 0; j < 5; j++) {
            char c = 'O';
            grid[i][j] = c;
        }
    }

    print_grid(grid);

    close_grid(grid);
}