#include <stdlib.h>
#include "grid.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#if DEBUG_BIN
#include <stdio.h>
#endif

#define PROBA_TWO   90

const uint128_tt BOX_MASK = 31;
const uint128_tt BOX_SIZE = 5;

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

uint8_t max_tile_bin(grid_t grid) {
    uint8_t maxi = 0;
    int i;
    for (i = 0; i < N * N; i++) {
        maxi = MAX(maxi, get_box(grid, i));
    }
    return maxi;
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



/*
 * Board encoding: 128 bits integer
 * Each box of the board is coded with five bits
 * There are 16 boxes, so the last 80 bits of the integer corresponds to
 * the state of the board
 *
 */
grid_t create_empty_grid_bin(void) {
    return 0L;
}

uint8_t get_box(grid_t grid, uint8_t n) {
#if DEBUG_BIN
    printf("GET_BOX(%u) = %u\n", n, (uint8_t) ((grid >> (n * BOX_SIZE)) & BOX_MASK));
#endif
    return (uint8_t) ((grid >> (n * BOX_SIZE)) & BOX_MASK);
}

grid_t set_box(grid_t grid, uint32_t n, uint8_t value) {
#if DEBUG_BIN
    printf("SET_BOX(%u, %u) = %u\n", n, value, (uint8_t) (value & BOX_MASK));
    printf("MASK: %lx\n", ((BOX_MASK << (n * BOX_SIZE))));
    printf("MASK: %lx\n", (~(BOX_MASK << (n * BOX_SIZE))));
#endif
    grid = grid & ((grid_t) (~(BOX_MASK << (n * BOX_SIZE))));
    return grid | (((grid_t) (value & BOX_MASK)) << (n * BOX_SIZE));
}


grid_t init_grid_bin(grid_t grid, uint8_t *nb_empty_box_out) {
    int i;
    uint8_t mask;
    uint8_t box_id;
    for (i = 0; i < 2; i++) {
        if (rand() % 100 > PROBA_TWO)
            mask = 0x02;
        else
            mask = 0x01;
        do {
            box_id = rand() % 16;
        } while (get_box(grid, box_id) != 0);
        grid = set_box(grid, box_id, mask);
    }
    *nb_empty_box_out = N * N - 2;
    return grid;
}


uint32_t bin_to_tile(uint8_t box) {
    uint32_t tile = 2;
    if (box == 0)
        return 1;
    else
        box--;
    while (box > 0) {
        tile *= 2;
        box--;
    }
    return tile;
}

row_t get_row(grid_t grid, uint8_t dir, uint8_t row_id) {
    int i;
    row_t row = 0;
    for (i = 0; i < N; i++) {
        switch (dir) {
        case NORTH:
            row = row | (get_box(grid, i * 4 + row_id) << (i * BOX_SIZE));
            break;
        case EAST:
            row = row << BOX_SIZE;
            row = row | get_box(grid, row_id * 4 + i);
            break;
        case SOUTH:
            row = row << BOX_SIZE;
            row = row | get_box(grid, i * 4 + row_id);
            break;
        case WEST:
            row = row | (get_box(grid, row_id * 4 + i) << (i * BOX_SIZE));
            break;
        }
    }
    return row;
}

grid_t set_row(grid_t grid, uint8_t dir, uint8_t row_id, uint32_t row) {
    int i;
    for (i = 0; i < 4; i++) {
        switch (dir) {
        case NORTH:
            grid = set_box(grid, i * 4 + row_id, row & BOX_MASK);
            row = row >> BOX_SIZE;
            break;
        case EAST:
            grid = set_box(grid, row_id * 4 + (N - i - 1), row & BOX_MASK);
            row = row >> BOX_SIZE;
            break;
        case SOUTH:
            grid = set_box(grid, (N - 1 - i) * 4 + row_id, row & BOX_MASK);
            row = row >> BOX_SIZE;
            break;
        case WEST:
            grid = set_box(grid, row_id * 4 + i, row & BOX_MASK);
            row = row >> BOX_SIZE;
            break;
        }
    }
    return grid;
}


row_t set_box_in_row(row_t row, uint8_t n, uint8_t val) {
#if DEBUG_BIN
    printf("MASK: %lx\n", ((BOX_MASK << (n * BOX_SIZE))));
    printf("MASK: %lx\n", (~(BOX_MASK << (n * BOX_SIZE))));
#endif
    row = row & ((row_t) (~(BOX_MASK << (n * BOX_SIZE))));
    return row | (((row_t) (val & BOX_MASK)) << (n * BOX_SIZE));
}

uint8_t get_box_in_row(row_t row, uint8_t n) {
    return (uint8_t) ((row >> (n * BOX_SIZE)) & BOX_MASK);
}
