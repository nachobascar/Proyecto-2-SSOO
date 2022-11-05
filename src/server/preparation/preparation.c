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

char char_to_pos(char character) {
    int pos;
    switch (character) {
        case 'A':
            pos = 0;
            break;
        case 'B':
            pos = 1;
            break;
        case 'C':
            pos = 2;
            break;
        case 'D':
            pos = 3;
            break;
        case 'E':
            pos = 4;
            break;
        default:
            break;
    }
    return pos;
}

void sort_range(int* range) {
    int first = range[0];
    int last = range[1];
    if (first > last) {
        range[0] = last;
        range[1] = first;
    }
}

void place_ship(char** board, char* start, char* end) {
    int start_pos[] = {(int) start[1] - '0' - 1, char_to_pos(start[0])};
    int end_pos[] = {(int) end[1] - '0' - 1, char_to_pos(end[0])};

    printf("Start position: %d, %d\n", start_pos[0], start_pos[1]);
    printf("End position: %d, %d\n", end_pos[0], end_pos[1]);

    if (start_pos[0] == end_pos[0]) {
        int range[] = {start_pos[1], end_pos[1]};
        sort_range(range);
        for (int i = range[0]; i <= range[1]; i++) {
            board[start_pos[1]][i] = 'O';
        }
    } else if (start_pos[1] == end_pos[1]) {
        for (int i = start_pos[0]; i <= end_pos[0]; i++) {
            board[i][start_pos[1]] = 'O';
        }
    }
}

void close_board(char** board) {
    for (int i = 0; i < 5; i++) {
        free(board[i]);
    }
    free(board);
}