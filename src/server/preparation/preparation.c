#include "preparation.h"

char** create_board() {
    char** grid = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        grid[i] = malloc(5 * sizeof(char));
        for (int j = 0; j < 5; j++) {
            char c = ' ';
            grid[i][j] = c;
        }
    }
    grid[3][3] = 'O';
    grid[4][3] = 'O';

    grid[2][1] = 'O';
    grid[3][1] = 'O';
    grid[4][1] = 'O';

    // grid[3][3] = 'O';
    // grid[4][3] = 'O';
    // grid[3][3] = 'O';
    // grid[4][3] = 'O';

    return grid;
}

int count_placed_ships(char** board) {
    int o_count = 0;
    int placed_ships = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == 'O') {
                o_count++;
            }
        }
    }

    switch (o_count) {
        case 2:
            placed_ships = 1;
            break;
        case 5:
            placed_ships = 2;
            break;
        case 9:
            placed_ships = 3;
            break;
        default:
            break;
    }
    return placed_ships;
}

void place_ship(char** board, char* start, char* end) {
    int placed_ships = count_placed_ships(board);
    printf("Placed ships: %d\n", placed_ships);
}

void close_board(char** board) {
    for (int i = 0; i < 5; i++) {
        free(board[i]);
    }
    free(board);
}