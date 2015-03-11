#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "ui.h"

/* 
   In this file for each direction there are four functions
   needed to compute the new grid. 
   This code could be factorised, but with sacrifice on
   the readability, so the choice has been made to 
   distinguish the functions depending on the direction.

   The four functions relative to south has been carefuly 
   commented. The algorithm of the other functions (corresponding
   to other directions) is similar. 
*/


/******************** MOVING TO SOUTH ********************/

/*
 *  From box (l, c), remove all gaps that are at the north of this box 
 *  until the next non-empty box is in (l-1, c)
 *
 *  Return true if the grid has been modified, false otherwise
 */
bool remove_gaps_south(uint16_t **grid, uint8_t l, uint8_t c) { 
    int k;
    /* If the box in (l-1, c) is not empty, there is no space to remove */
    if ((l - 1) >= 0 && grid[l-1][c] != 0) {
        /* As the grid has not been modified, return false */
        return false;
    }
    /* Starting from two boxes above, we look for the next non-empty box */
    k = l - 2;
    while (k >= 0 && grid[k][c] == 0) {
        k--;
    }
    /* If we are out of the grid, there is no non-empty box to move */
    if (k < 0) {
        /* The grid has not been modified, so we return false */
        return false;
    /* Else, meaning if we have found a non-empty box at the north of (l, c) */
    } else {
        /* We move it to (l-1, c) which is empty */
        grid[l-1][c] = grid[k][c];
        grid[k][c] = 0;
        /* And as we have modified the grid, return true */
        return true;
    }
}

/*
 *  Try to merge (l, c) with (l-1, c)
 *
 *  Return true if the merge is successful, false otherwise
 */
bool merge_tiles_south(uint16_t **grid, uint8_t l, uint8_t c) {
    /* If we are at the top of the grid or if the values of the two boxes are
       different */
    if (l - 1 < 0 || grid[l][c] != grid[l-1][c] || grid[l][c] == 0) {
        /* Then no merge */
        return false;
    } else {
        /* Else performing the merge */
        grid[l][c] *= 2;
        grid[l-1][c] = 0;
        return true;
    }
}

/*
 *  From the bottom of column c, try to find the first non-empty box
 *  If there is such a box, bring it to the southest box
 *
 *  Return true iif the grid has been modified (e.g. there was at least one
 *  non-empty box and not already in the southest box)
 */
bool bring_first_tile_south(uint16_t **grid, uint8_t c) {
    int l;
    /* If the southest box is not empty, nothing to do */
    if (grid[N-1][c] != 0) {
        return false;
    }
    /* Trying to find the first non-empty grid */
    for (l = N - 2; l >= 0; l--) {
        /* If found */
        if (grid[l][c] != 0) {
            /* Moving it to the south */
            grid[N-1][c] = grid[l][c];
            grid[l][c] = 0;
            /* The grid has been modified */
            return true;
        }
    }
    /* At the end of this loop, if we haven't 
       find a non-empty box, return false */
    return false;
}

/* 
 *  Global function implementing move to south
 *
 *  Return true iif a move has been computed, ie the grid
 *  has been modified
 */
bool move_south(uint16_t **grid) {
    int l, c;
    bool has_changed = false;
    /* Treating columns one by one */
    for (c = 0; c < N; c++) {
        /* First, moving the first tile to the south 
            (ie removing all empty-boxes from
            the south to the first non-empty box) */
        if (bring_first_tile_south(grid, c)) 
            has_changed = true;
        /* Then, for each box from south to north */
        for (l = N-1; l > 0; l--) {
            /* We remove all empty-boxes above the current box */
            if (remove_gaps_south(grid, l, c)) 
                has_changed = true;
            /* We try to merge with the box above */
            if (merge_tiles_south(grid, l, c)) {
                has_changed = true;
                /* And we remove gaps once again if we merged */
                remove_gaps_south(grid, l, c);
            }
        }
    }
    return has_changed;
}

/*********************************************************/


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

/*********************************************************/


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

/*********************************************************/


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

/*********************************************************/

/*
 *  Perform the move given by dir on the grid.
 *
 *  Return true if the grid has changed (merge or move), 
 *  false otherwise
 */
bool move(uint16_t **grid, uint8_t dir) {
    /* This function calls the relative move function depending 
       on the direction given as a parameter */
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

/*
 *  This function adds a random tile (either 2 or 4) in a random box
 */
void add_tile(uint16_t **grid) {
    int i, j;
    /* Randomize the two coordinates i and j */
    do {
        i = rand() % N;
        j = rand() % N;
    /* until we found an empty box */
    } while (grid[i][j] != 0);
    /* Putting a random value (2 or 4) */
    grid[i][j] = (rand() % 100 < 90)?2:4;
    return;
}

/*
 *  Return true iif no move is possible, meaning if there is no empty box
 *  _AND_ no merge is possible
 */
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
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (j + 1 < N && grid[i][j] == grid[i][j+1])
                return false;
            if (i + 1 < N && grid[i][j] == grid[i+1][j])
                return false;
        }
    }

    return true;
}

/*
 *  Try to merge (l, c) with (l-1, c)
 *
 *  Return true if the merge is successful, false otherwise
 */
row_t merge_tiles_bin(row_t row, uint8_t n) {
    /* If we are at the top of the grid or if the values of the two boxes are
       different */
    if (n >= N - 1 || get_box_in_row(row, n) != get_box_in_row(row, n + 1) || get_box_in_row(row, n) == 0)
        return row;
    row = set_box_in_row(row, n, get_box_in_row(row, n) + 1);
    row = set_box_in_row(row, n + 1, 0);
    return row;
}

row_t remove_gaps_bin(row_t row, uint8_t n) {
    int i;
    if (n == N - 1 || get_box_in_row(row, n) == 0 || get_box_in_row(row, n + 1) != 0) {
        return row;
    }
    i = n + 2;
    while (i < N && get_box_in_row(row, i) == 0)
        i += 1;
    if (i >= N)
        return row;
    row = set_box_in_row(row, n + 1, get_box_in_row(row, i));
    row = set_box_in_row(row, i, 0);
    return row;
}

row_t bring_first_tile(row_t row) {
    int i = 0;
    while ((row & BOX_MASK) == 0 && i < N) {
        row = row >> BOX_SIZE;
        i++;
    }
    return row;
}

/* /!\ Convention: les bits de poids faible de row codent la première case 
    (celle de la direction du mouvement). Par exemple, si la direction est 
    NORTH, alors les bits de poids faible de row codent la case la plus au nord
 */
grid_t move_bin(grid_t grid, uint8_t dir, uint8_t *nb_merges_out) {
    int i, j;
    uint8_t nb_merges = 0;
    row_t row, saved_row;
    for (i = 0; i < N; i++) {
        row = get_row(grid, dir, i);
        row = bring_first_tile(row);   
        for (j = 0; j < N - 1; j++) {
            row = remove_gaps_bin(row, j);
            saved_row = row;
            row = merge_tiles_bin(row, j);
            if (row != saved_row)
                nb_merges++;
            row = remove_gaps_bin(row, j);
        }
        grid = set_row(grid, dir, i, row);
    }
    *nb_merges_out = nb_merges;
    return grid;
}

grid_t add_tile_bin(grid_t grid) {
    int i, j;
    /* Randomize the two coordinates i and j */
    do {
        i = rand() % N;
        j = rand() % N;
    /* until we found an empty box */
    } while (get_box(grid, i * N + j) != 0);
    /* Putting a random value (2 or 4) */
    grid = set_box(grid, i * N + j, (rand() % 100 < 90)?2:4);
    return grid;
}

bool merge_possible_bin(grid_t grid) {
    int i, j;
    /* If two adjacent boxes have same value, game is not over */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (j + 1 < N && get_box(grid, i * N + j) == get_box(grid, i * N + j + 1))
                return true;
            if (i + 1 < N && get_box(grid, i * N + j) == get_box(grid, (i+1) * N + j))
                return true;
        }
    }

    return false;
}
