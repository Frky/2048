#include <stdio.h>
#include "game.h"

uint16_t **create_empty_grid(void) {
    uint16_t **grid = malloc(N * sizeof(uint16_t *));
    uint16_t i;
    for (i = 0; i < N; i++) {
        grid[i] = calloc(N * sizeof(uint16_t), 0);
    }
    return grid;
}

/*
 *  Take an empty grid as a parameter and set two
 *  boxes with a 2 and two boxes with a 4
 */
void init_grid(uint16_t **grid) {
    int i, j, k;
    for (k = 0; k < N; k++) {
        do {
            i = rand() % N;
            j = rand() % N;
        } while (grid[i][j] != 0);
        grid[i][j] = 2 * (k / 2 + 1);
    }
    return;
}

void print_line_break(void) {
    int i;
    printf(" ");
    for (i = 0; i < N; i++) {
        printf("--- ");
    }
    printf("\n");
    return;
}

void display_grid(uint16_t **grid) {
    int i, j;
    print_line_break();
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("|");
            if (grid[i][j]) {
                printf(" %i ", grid[i][j]);
            } else {
                printf("   ");
            }
        }
        printf("|\n");
        print_line_break();
    }
    return;
}

int nb_not_empty_boxes(uint16_t **grid, int l, int dir) {
    int j, n = 0;
    for (j = 0; j < N; j++) {
        if (dir == WEST || dir == EAST) {
            n += (grid[l][j] != 0)?1:0;
        } else {
            n += (grid[j][l] != 0)?1:0;
        }
    }
    return n;
}

int move_line(uint16_t **grid, uint8_t dir, uint8_t i) {
    int dx, dy;
    int x, y, k;
    uint16_t val;
    switch (dir) {
    case NORTH:
        x = N - 1;  y = i;
        dx = -1;     dy = 0;
        break;
    case EAST:
        x = i;      y = 0;
        dx = 0;     dy = 1;
        break;
    case SOUTH:
        x = 0;      y = i;
        dx = 1;     dy = 0;
        break;
    case WEST:
        x = i;      y =  N - 1;
        dx = 0;    dy = -1;
        break;
    }
    for (k = 0; k < N - 1; k++) {
        val = grid[x + k*dx][y + k*dy];
        if (val == 0) {
            continue;
        }
        if (grid[x + (k+1) * dx][y + (k+1) * dy] == 0) {
            grid[x + (k + 1) * dx][y + (k+1) * dy] = grid[x + k*dx][y + k*dy];
            grid[x + k*dx][y + k*dy] = 0;
            k = -1;
        } else if (grid[x + (k+1) * dx][y + (k+1) * dy] == val) {
            grid[x + (k + 1) * dx][y + (k+1) * dy] += grid[x + k*dx][y + k*dy];
            grid[x + k*dx][y + k*dy] = 0;
            k = -1;
        }
    }
    return 0;
}

int move(uint16_t **grid, uint8_t dir) {
    int i;
    for (i = 0; i < N; i++) {
        move_line(grid, dir, i);
    }
    return 1;
}

void add_tile(uint16_t **grid) {
    int i, j;
    do {
        i = rand() % N;
        j = rand() % N;
    } while (grid[i][j] != 0);
    grid[i][j] = 2*(rand() % 2 + 1);
}
