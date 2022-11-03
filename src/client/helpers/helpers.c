#include "helpers.h"

// Print 5x5 grid with letters in columns and numbers in rows
void print_grid(char** grid) {
    printf(ANSI_COLOR_RED "    A   B   C   D   E\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BLUE"  - - - - - - - - - - - \n"ANSI_COLOR_RESET);
    for (int i = 0; i < 5; i++) {
        printf(ANSI_COLOR_RED"%d" ANSI_COLOR_BLUE" |"ANSI_COLOR_RESET, i + 1);
        for (int j = 0; j < 5; j++) {
            printf(" %c " ANSI_COLOR_BLUE "|" ANSI_COLOR_RESET , grid[i][j]);
        }
        printf(ANSI_COLOR_RED" %d\n" ANSI_COLOR_RESET, i + 1);
        printf(ANSI_COLOR_BLUE"  - - - - - - - - - - - \n"ANSI_COLOR_RESET);
    }
    printf(ANSI_COLOR_RED "    A   B   C   D   E\n" ANSI_COLOR_RESET);
}