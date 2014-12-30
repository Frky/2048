#include <stdlib.h>
#include "grid.h"

/*
 *  Allocate a grid of size N * N initialized with zeros
 */
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
 *  boxes with either 2  or 4
 */
void init_grid(uint16_t **grid) {
    int i, j, k;
    for (k = 0; k < 2; k++) {
        /* Randomize two coordinates until 
            we found an empty box */
        do {
            i = rand() % N;
            j = rand() % N;
        } while (grid[i][j] != 0);
        /* Putting randomly either 2 or 4 */
        grid[i][j] = 2 * (rand() % 2 + 1);
    }
    return;
}

/*
 *  Creates a copy of the grid
 *  This function allocaes memory
 */
uint16_t **copy_grid(uint16_t **grid) {
    uint16_t **new_grid = create_empty_grid();
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            new_grid[i][j] = grid[i][j];
        }
    }
    return new_grid;
}

/*
 *  Return the maximum value of a tile in the grid
 */
uint16_t max_tile(uint16_t **grid) {
    uint16_t max = 0;
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            max = (max<grid[i][j])?grid[i][j]:max;
        }
    }
    return max;
}

/*
 *  Free a previously allocated grid
 */
void free_grid(uint16_t **grid) {
    int i;
    for (i = 0; i < N; i++) {
        free(grid[i]);
    }
    free(grid);
    return;
}
