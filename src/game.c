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


/******************** MOVING TO SOUTH ********************/

bool remove_gaps_south(uint16_t **grid, uint8_t l, uint8_t c) { 
    int k;
    if ((l - 1) >= 0 && grid[l-1][c] != 0) {
        return false;
    }
    k = l - 2;
    while (k >= 0 && grid[k][c] == 0) {
        k--;
    }
    if (k < 0) {
        return false;
    } else {
        grid[l-1][c] = grid[k][c];
        grid[k][c] = 0;
        return true;
    }
}

bool merge_tiles_south(uint16_t **grid, uint8_t l, uint8_t c) {
    if (l - 1 < 0 || grid[l][c] != grid[l-1][c] || grid[l][c] == 0) {
        return false;
    } else {
        grid[l][c] *= 2;
        grid[l-1][c] = 0;
        return true;
    }
}

bool bring_first_tile_south(uint16_t **grid, uint8_t c) {
    int l;
    if (grid[N-1][c] != 0) {
        return false;
    }
    for (l = N - 2; l >= 0; l--) {
        if (grid[l][c] != 0) {
            grid[N-1][c] = grid[l][c];
            grid[l][c] = 0;
            return true;
        }
    }
    return false;
}

bool move_south(uint16_t **grid) {
    int l, c;
    bool has_changed = false;
    for (c = 0; c < N; c++) {
        if (bring_first_tile_south(grid, c)) 
            has_changed = true;
        for (l = N-1; l > 0; l--) {
            if (remove_gaps_south(grid, l, c)) 
                has_changed = true;
            if (merge_tiles_south(grid, l, c)) {
                has_changed = true;
                remove_gaps_south(grid, l, c);
            }
        }
    }
    return has_changed;
}

/******************** MOVING TO NORTH ********************/

bool remove_gaps_north(uint16_t **grid, uint8_t l, uint8_t c) { 
    int k;
    if ((l + 1) < N && grid[l+1][c] != 0) {
        return false;
    }
    k = l + 2;
    while (k < N && grid[k][c] == 0) {
        k++;
    }
    if (k >= N) {
        return false;
    } else {
        grid[l+1][c] = grid[k][c];
        grid[k][c] = 0;
        return true;
    }
}

bool merge_tiles_north(uint16_t **grid, uint8_t l, uint8_t c) {
    if (l + 1 >= N || grid[l][c] != grid[l+1][c] || grid[l][c] == 0) {
        return false;
    } else {
        grid[l][c] *= 2;
        grid[l+1][c] = 0;
        return true;
    }
}

bool bring_first_tile_north(uint16_t **grid, uint8_t c) {
    int l;
    if (grid[0][c] != 0) {
        return false;
    }
    for (l = 1; l < N; l++) {
        if (grid[l][c] != 0) {
            grid[0][c] = grid[l][c];
            grid[l][c] = 0;
            return true;
        }
    }
    return false;
}

int move_north(uint16_t **grid) {
    int l, c;
    bool has_changed = false;
    for (c = 0; c < N; c++) {
        if (bring_first_tile_north(grid, c)) 
            has_changed = true;
        for (l = 0; l < N - 1; l++) {
            if (remove_gaps_north(grid, l, c)) 
                has_changed = true;
            if (merge_tiles_north(grid, l, c)) {
                has_changed = true;
                remove_gaps_north(grid, l, c);
            }
        }
    }
    return has_changed;
}

/******************** MOVING TO EAST ********************/

bool remove_gaps_east(uint16_t **grid, uint8_t l, uint8_t c) { 
    int k;
    if ((c - 1) >= 0 && grid[l][c-1] != 0) {
        return false;
    }
    k = c - 2;
    while (k >= 0 && grid[l][k] == 0) {
        k--;
    }
    if (k < 0) {
        return false;
    } else {
        grid[l][c-1] = grid[l][k];
        grid[l][k] = 0;
        return true;
    }
}

bool merge_tiles_east(uint16_t **grid, uint8_t l, uint8_t c) {
    if (c - 1 < 0 || grid[l][c] != grid[l][c-1] || grid[l][c] == 0) {
        return false;
    } else {
        grid[l][c] *= 2;
        grid[l][c-1] = 0;
        return true;
    }
}

bool bring_first_tile_east(uint16_t **grid, uint8_t l) {
    int c;
    if (grid[l][N-1] != 0) {
        return false;
    }
    for (c = N - 2; c >= 0; c--) {
        if (grid[l][c] != 0) {
            grid[l][N-1] = grid[l][c];
            grid[l][c] = 0;
            return true;
        }
    }
    return false;
}

int move_east(uint16_t **grid) {
    int l, c;
    bool has_changed = false;
    for (l = 0; l < N; l++) {
        if (bring_first_tile_east(grid, l)) 
            has_changed = true;
        for (c = N - 1; c > 0; c--) {
            if (remove_gaps_east(grid, l, c)) 
                has_changed = true;
            if (merge_tiles_east(grid, l, c)) {
                has_changed = true;
                remove_gaps_east(grid, l, c);
            }
        }
    }
    return has_changed;
}

/******************** MOVING TO WEST ********************/

bool remove_gaps_west(uint16_t **grid, uint8_t l, uint8_t c) { 
    int k;
    if ((c + 1) < N && grid[l][c+1] != 0) {
        return false;
    }
    k = c + 2;
    while (k < N && grid[l][k] == 0) {
        k++;
    }
    if (k >= N) {
        return false;
    } else {
        grid[l][c+1] = grid[l][k];
        grid[l][k] = 0;
        return true;
    }
}

bool merge_tiles_west(uint16_t **grid, uint8_t l, uint8_t c) {
    if (c + 1 >= N || grid[l][c] != grid[l][c+1] || grid[l][c] == 0) {
        return false;
    } else {
        grid[l][c] *= 2;
        grid[l][c+1] = 0;
        return true;
    }
}

bool bring_first_tile_west(uint16_t **grid, uint8_t l) {
    int c;
    if (grid[l][0] != 0) {
        return false;
    }
    for (c = 1; c <= N; c++) {
        if (grid[l][c] != 0) {
            grid[l][0] = grid[l][c];
            grid[l][c] = 0;
            return true;
        }
    }
    return false;
}

bool move_west(uint16_t **grid) {
    int l, c;
    bool has_changed = false;
    for (l = 0; l < N; l++) {
        if (bring_first_tile_west(grid, l)) 
            has_changed = true;
        for (c = 0; c < N - 1; c++) {
            if (remove_gaps_west(grid, l, c)) 
                has_changed = true;
            if (merge_tiles_west(grid, l, c)) {
                has_changed = true;
                remove_gaps_west(grid, l, c);
            }
        }
    }
    return has_changed;
}


bool move(uint16_t **grid, uint8_t dir) {
    switch (dir) {
    case SOUTH:
        return move_south(grid);
    case NORTH:
        return move_north(grid);
    case EAST:
        return move_east(grid);
    case WEST:
        return move_west(grid);
    default: 
        return false;
    }
}


void add_tile(uint16_t **grid) {
    int i, j;
    do {
        i = rand() % N;
        j = rand() % N;
    } while (grid[i][j] != 0);
    grid[i][j] = 2*(rand() % 2 + 1);
}


bool game_over(uint16_t **grid) {
    int i, j;
    /* If there is an empty box (at least) game is not over */
    for (i = 0 ; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (grid[i][j] == 0) 
                return false;
        }
    }
    /* If two adjacent boxes have same value, game is not over */
    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - 1; j++) {
            if (grid[i][j] == grid[i][j+1] || grid[i][j] == grid[i+1][j])
                return false;
        }
    }

    return true;
}
