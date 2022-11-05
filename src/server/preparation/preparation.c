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
    if (character >= 'A' && character <= 'E') {
        pos = character - 'A';
    } else {
        pos = character - 'a';
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

int check_pos(char* start, char* end) {
    if (strlen(start) != 2 || strlen(end) != 2) {
        return 0;
    }
    if (!((start[0] >= 'A' && start[0] <= 'E') || (start[0] >= 'a' && start[0] <= 'e'))
        || !((end[0] >= 'A' && end[0] <= 'E') || (end[0] >= 'a' && end[0] <= 'e'))) {
        return 0;
    }
    if (!(start[1] >= '1' && start[1] <= '5' && end[1] >= '1' && end[1] <= '5')) {
        return 0;
    }
    return 1;
}

int check_ship_length(int ship_length, int placed_ships) {
    if ((ship_length == 2 && placed_ships == 0)
        || (ship_length == 3 && placed_ships == 1)
        || (ship_length == 4 && placed_ships == 2)) {
        return 1;
    }
    return 0;
}

int place_ship(char** board, char* start, char* end) {
    if (!check_pos(start, end)) {
        return -1;
    }

    int start_pos[] = {(int) start[1] - '0' - 1, char_to_pos(start[0])};
    int end_pos[] = {(int) end[1] - '0' - 1, char_to_pos(end[0])};

    if (start_pos[0] == end_pos[0]) {
        int range[] = {start_pos[1], end_pos[1]};
        sort_range(range);
        int ship_length = range[1] - range[0] + 1;
        if (!check_ship_length(ship_length, count_placed_ships(board))) {
            return -1;
        }
        for (int i = range[0]; i <= range[1]; i++) {
            if (board[start_pos[0]][i] == 'O') {
                return -1;
            }
        }
        for (int i = range[0]; i <= range[1]; i++) {
            board[start_pos[0]][i] = 'O';
        }
        print_grid(board);
        return 0;

    } else if (start_pos[1] == end_pos[1]) {
        int range[] = {start_pos[0], end_pos[0]};
        sort_range(range);
        int ship_length = range[1] - range[0] + 1;
        if (!check_ship_length(ship_length, count_placed_ships(board))) {
            return -1;
        }
        for (int i = range[0]; i <= range[1]; i++) {
            if (board[start_pos[0]][i] == 'O') {
                return -1;
            }
        }
        for (int i = range[0]; i <= range[1]; i++) {
            board[i][start_pos[1]] = 'O';
        }
        print_grid(board);
        return 0;

    } else {
        return -1;
    }

    return -1;
}

void start_preparation(char** board) {
    int confirmed_ships = 0;

    char* place_ship_menu = "----- Menu de preparación -----\n\n"
        "Ingresa las coordenadas de inicio y fin del barco\n"
        "\t- Pueden estar separadas por un espacio o por Enter\n"
        "\t- Deben ser de la forma Letra Número (por ejemplo A1)\n"
        "\t- La letra puede estar en mayúsculas o minúsculas\n";
    printf("%s", place_ship_menu);

    while (!confirmed_ships) {
        if (count_placed_ships(board) != 3) {
            char* prompt = "Ingresa las coordenadas de inicio y fin del barco\n";
            printf("%s", prompt);
            char start[20];
            char end[20];
            scanf("%s %s", start, end);
            int status = place_ship(board, start, end);
            printf("\n");
            if (status == 0) {
                print_grid(board);
            } else {
                char* error_msg = "Las coordenadas ingresadas son inválidas. Recuerda lo siguiente\n"
                "\t- Seguir el formato para ingresar coordenadas\n"
                "\t- Los barcos solo pueden estar horizontal o verticalmente\n"
                "\t- Primero debes ingresar un barco de largo 2, luego 3 y luego 4\n"
                "\t- Un barco no puede chocar con otro\n";
            printf("%s", error_msg);
            }
        } else {
            char* place_ship_menu = "";
            printf("%s\n", place_ship_menu);
            break;
        }
    }
}

void close_board(char** board) {
    for (int i = 0; i < 5; i++) {
        free(board[i]);
    }
    free(board);
}