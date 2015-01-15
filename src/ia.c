#include <stdlib.h>
#include <stdio.h>

#include "grid.h" 
#include "game.h"
#include "ia.h"

#define INFINITE    2147483647  // = 2^31 - 1
#define MAX_DEPTH   3           // 3 is enough to often reach 2048
#define NB 15


/*
 *  Return the difference between two tiles, absolute value
 */
uint16_t tile_gap(uint16_t a, uint16_t b) {
    if (a < b) 
        return b - a;
    else
        return a - b;
}


/*
 *  Return the number of empty boxes in the grid
 */
uint8_t nb_empty_box(uint16_t **grid) {
    uint8_t n = 0;
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (grid[i][j] == 0)
                n++;
        }
    }
    return n;
}

/* 
 *  Return an integer corresponding to the position of the 
 *  highest tile (if several, use the most advantageous 
 *  position).
 *
 *  Return values:
 *      0: the max tile is not in a corner nor at border
 *      1: the max tile is at border
 *      2: the max tile is in a corner
 */
uint8_t tile_position(uint16_t **grid, uint16_t tile) {
    uint8_t pos = 0, new_pos;
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (grid[i][j] == tile) {
                new_pos = 0;
                if (j == 0 || j == N-1)
                    new_pos++;
                if (i == 0 || i == N-1) 
                    new_pos++;
                if (pos < new_pos)
                    pos = new_pos;
            }
        }
    }
    return pos;
}

/*
 *  Return the log in base two of the tile given in parameter
 */
uint8_t log_two(uint16_t tile) {
    uint8_t res = 1;
    uint16_t senti = 2;
    while (senti < tile) {
        senti *= 2;
        res++;
    }
    return res;
}

/*
 *  Evaluate the compacity of the grid (heuristic).
 *  The more gaps there are between two adjacent tiles, 
 *  the more this value is high.
 *  A compacity of 0 indicates that all tiles have same value
 */
int compacity(uint16_t **grid) {
    int gap = 0, nb_gaps = 0;
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i + 1 < N) { 
                gap += 2*tile_gap(grid[i][j], grid[i+1][j]);
                nb_gaps++;
            }
            if (j + 1 < N) { 
                gap += 2*tile_gap(grid[i][j], grid[i][j-1]);
                nb_gaps++;
            }
        }
    }
    if (nb_gaps == 0) 
        return 0;
    else
        return gap / (nb_gaps*2);
}

/*
 *  Evaluate que quality of the grid
 */
int evaluate(uint16_t **grid) {
    /* Getting the maximum value in the grid */
    uint8_t max = max_tile(grid);
    uint8_t lmax = log_two(max);
    /* Is the max value in a corner (2), 
       in a border (1) or in the middle (0) ? */
    uint8_t pos_of_max = tile_position(grid, max);
    int value = lmax + 3*pos_of_max + nb_empty_box(grid);
    if (game_over(grid)) 
            return -INFINITE;
    value -= compacity(grid);
    return value;
}


int random_move(void) {
    return rand() % 4;
}

int explore_game(uint16_t **grid, int depth);

int explore_moves(uint16_t **grid, int depth) {
    int dir;
    uint16_t **grid_copy;
    int val, max_val = -INFINITE;
    for (dir = 0; dir < 4; dir++) {
        grid_copy = copy_grid(grid);
        if (move(grid_copy, dir)) {
            val = explore_game(grid_copy, depth + 1);
            if (val > max_val) {
                max_val = val;
            }
        }
        free_grid(grid_copy);
    }
    return max_val;
}

int explore_game(uint16_t **grid, int depth) {
    int i, j, k, n;
    uint16_t **grid_copy;
    float av_val = 0;

    if (depth >= MAX_DEPTH) {
        return evaluate(grid);
    }

    for (n = 0; n < NB; n++) {
        do {
            i = rand() % N;
            j = rand() % N;
        } while (grid[i][j] != 0);
        grid_copy = copy_grid(grid);
        grid_copy[i][j] = (rand() % 100 < 90)?2:4;
        av_val += explore_moves(grid_copy, depth);
        free_grid(grid_copy);
    }
    return av_val / NB;

#if 0
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (grid[i][j] != 0) 
                continue;
            for (k = 1; k <= 2; k++) {
                grid_copy = copy_grid(grid);
                grid_copy[i][j] = 2*k;
#if 1
                if (game_over(grid_copy)) {
                    free_grid(grid_copy);
                    return -INFINITE;
                }
#endif
                val = explore_moves(grid_copy, depth);
                if (val < min_val) {
                    min_val = val;
                }
                free_grid(grid_copy);
            }
        }
    }
    return min_val;
#endif
}

int best_move(uint16_t **grid) {
    int dir, best_dir;
    int val, max_val = -INFINITE;
    uint16_t **grid_copy;
    for (dir = 0; dir < N; dir++) {
        grid_copy = copy_grid(grid);
        if (move(grid_copy, dir)) {
            val = explore_game(grid_copy, 1);
            if (val >= max_val) {
                max_val = val;
                best_dir = dir;
            }
        }
        free_grid(grid_copy);
    }
    return best_dir;
}

