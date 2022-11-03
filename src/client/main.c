#include "helpers/helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    printf("Hello, World!\n Im the client\n");

    srand(time(NULL)); 

    char** grid = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) {
        grid[i] = malloc(5 * sizeof(char));
        for (int j = 0; j < 5; j++) {
            int r = rand() % 4;
            char c;
            switch (r)
            {
            case 0:
                c = ' ';
                break;
            case 1:
                c = 'X';
                break;
            case 2:
                c = 'O';
                break;
            case 3:
                c = '-';
                break;
            
            default:
                break;
            }
            grid[i][j] = c;
        }
    }


    print_grid(grid);

    return 0;
}