#include "preparation.h"

char** create_board() {
    char** grid = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        grid[i] = malloc(5 * sizeof(char));
        for (int j = 0; j < 5; j++) {
            char c = 'O';
            grid[i][j] = c;
        }
    }

    return grid;
}

void close_board(char** board) {
    for (int i = 0; i < 5; i++) {
        free(board[i]);
    }
    free(board);
}