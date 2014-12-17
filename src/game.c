#include <stdio.h>
#include <stdbool.h>
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

#if 0
int remove_gaps(uint16_t **grid, uint8_t l, uint8_t c, uint8_t dir) {
    int k;
    switch (dir) {
    case NORTH:
        if ((l + 1) < N && grid[l+1][c] != 0) {
            return 0;
        }
        k = l + 2;
        while (k < N && grid[k][c] == 0) {
            k++;
        }
        if (k >= N) {
            return 0;
        } else {
            grid[l+1][c] = grid[k][c];
            grid[k][c] = 0;
        }
        break;
    case WEST:
        if (grid[l][c + 1] != 0) {
            return 0;
        }
        k = c + 2;
        while (k < N && grid[l][k] == 0) {
            k++;
        }
        if (k == N) {
            return 0;
        } else {
            grid[l][c + 1] = grid[l][k];
            grid[l][k] = 0;
        }
        break;
    case SOUTH:
    case EAST:
        return 0;
        break;
    }
    return 0;
}

int merge_tiles(uint16_t **grid, uint8_t c, uint8_t l, uint8_t dir) {
    switch (dir) {
    case NORTH:
        if ((l + 1) < N && grid[l][c] == grid[l+1][c]) {
            grid[l][c] *= 2;
            grid[l+1][c] = 0;
            return 1;
        } else {
            return 0;
        }
        break;
    case WEST:
        if ( (c + 1) < N && grid[l][c] == grid[l][c+1]) {
            grid[l][c] *= 2;
            grid[l][c + 1] = 0;
            return 1;
        } else {
            return 0;
        }
        break;
    case SOUTH:
        if ((l - 1) >= 0 && grid[l][c] == grid[l-1][c]) {
            grid[l][c] *= 2;
            grid[l-1][c] = 0;
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

int bring_first_tile(uint16_t **grid, uint8_t d, uint8_t dir) {
    int i;
    switch (dir) {
    case NORTH:
        if (grid[0][d] != 0) {
            return 0;
        }
        for (i = 1; i < N; i++) {
            if (grid[i][d] != 0) {
                grid[0][d] = grid[i][d];
                grid[i][d] = 0;
                return 1;
            }
        }
        break;
    case WEST:
        if (grid[d][0] != 0) {
            return 0;
        }
        for (i = 1; i < N; i++) {
            if (grid[d][i] != 0) {
                grid[d][0] = grid[d][i];
                grid[d][i] = 0;
                return 1;
            }
        }
        break;
    case SOUTH: 
        break;
    }
    return 0;
}

int move_line(uint16_t **grid, uint8_t dir, uint8_t i) {
    int k;
    bring_first_tile(grid, i, dir);
    for (k = 0; k < N; k++) {
        if (dir == NORTH) {
            remove_gaps(grid, k, i, dir);
        } else if (dir == WEST || dir == EAST) {
            remove_gaps(grid, i, k, dir);
        } else if (dir == SOUTH) {
            remove_gaps(grid, N - 1 - k, i, dir);
        }
        if (dir == NORTH) {
            if (merge_tiles(grid, i, k, dir)) {
                remove_gaps(grid, k, i, dir);
            }
        } else if (dir == WEST || dir == EAST) {
            if (merge_tiles(grid, k, i, dir)) {
                remove_gaps(grid, i, k, dir);
            }
        } else if (dir == SOUTH) {
            if (merge_tiles(grid, N - 1 - k, i, dir)) {
                remove_gaps(grid, i, N - 1 - k, dir);
            }
        }
    }
#if 0
    for (k = 0; k < N; k++) {
        if (grid[x + k * dx][y + k * dy] == 0) {
            just_merged = false;
            for (l = 0; l < prev; l++) {
                grid[x + (k-l) * dx][y + (k-l) * dy] = grid[x + (k-l-1) * dx][y + (k-l-1) * dy];
            }
            grid[x + (k-l) * dx][y + (k-l) * dy] = 0;
        } else if (k > 0 && !just_merged && grid[x + k * dx][y + k * dy] == grid[x + (k-1) * dx][y + (k-1) * dy]) {
            grid[x + k * dx][y + k * dy] += grid[x + (k-1)*dx][y + (k-1)*dy];
            grid[x + (k-1)*dx][y + (k-1)*dy] = 0;
            just_merged = true;
        } else {
            just_merged = false;
            prev += 1;
        }
    }
#endif
    return 0;
}
#endif

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
    if (l - 1 < 0 || grid[l][c] != grid[l-1][c]) {
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

int move_south(uint16_t **grid) {
    int l, c;
    for (c = 0; c < N; c++) {
        bring_first_tile_south(grid, c);
        for (l = N-1; l > 0; l--) {
            remove_gaps_south(grid, l, c);
            if (merge_tiles_south(grid, l, c)) {
                remove_gaps_south(grid, l, c);
            }
        }
    }
    return 1;
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
    if (l + 1 >= N || grid[l][c] != grid[l+1][c]) {
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
    for (c = 0; c < N; c++) {
        bring_first_tile_north(grid, c);
        for (l = 0; l < N - 1; l++) {
            remove_gaps_north(grid, l, c);
            if (merge_tiles_north(grid, l, c)) {
                remove_gaps_north(grid, l, c);
            }
        }
    }
    return 1;
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
    if (c - 1 < 0 || grid[l][c] != grid[l][c-1]) {
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
    for (l = 0; l < N; l++) {
        bring_first_tile_east(grid, l);
        for (c = N - 1; c > 0; c--) {
            remove_gaps_east(grid, l, c);
            if (merge_tiles_east(grid, l, c)) {
                remove_gaps_east(grid, l, c);
            }
        }
    }
    return 1;
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
    if (c + 1 >= N || grid[l][c] != grid[l][c+1]) {
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

int move_west(uint16_t **grid) {
    int l, c;
    for (l = 0; l < N; l++) {
        bring_first_tile_west(grid, l);
        for (c = 0; c < N - 1; c++) {
            remove_gaps_west(grid, l, c);
            if (merge_tiles_west(grid, l, c)) {
                remove_gaps_west(grid, l, c);
            }
        }
    }
    return 1;
}


int move(uint16_t **grid, uint8_t dir) {
    switch (dir) {
    case SOUTH:
        move_south(grid);
        break;
    case NORTH:
        move_north(grid);
        break;
    case EAST:
        move_east(grid);
        break;
    case WEST:
        move_west(grid);
        break;
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
