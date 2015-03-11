#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "src/game.h"
#include "src/ui.h"
#include "src/ia.h"

#define     USE_BIN 1
#define     IA      1
#define     RANDOM  0
#define     DEBUG   1

int main(void) {
    int dir;
    srand(time(NULL));
    uint8_t nb_empty_box, nb_merge;
#if USE_BIN 
    /* Creation of a grid */
    grid_t grid = create_empty_grid_bin();
    /* Grid backup */
    grid_t saved_grid;
    /* Initialisation of the grid */
    grid = init_grid_bin(grid, &nb_empty_box);
    /* Display the grid */
    display_grid_bin(grid);
    /* Main loop for a game */
    while (nb_empty_box > 0 || merge_possible_bin(grid)) {
        do {
            saved_grid = grid;
            /* While the move does not lead to a change on the grid */
            // dir = random_move();
            dir = best_move_bin(grid, nb_empty_box);
            /* Trying to play the move */
            grid = move_bin(grid, dir, &nb_merge);
            nb_empty_box += nb_merge;
        } while (grid == saved_grid);
        /* Add tile */
        grid = add_tile_bin(grid);
        nb_empty_box--;
        /* Display the new grid */
        display_grid_bin(grid);
        printf("Empty boxes: %u\n", nb_empty_box);
    }
#else
    /* Creation of a grid */
    uint16_t **grid = create_empty_grid();
    /* Initialisation of the grid */
    init_grid(grid);
    /* Display the grid */
    display_grid(grid);
#if DEBUG
    printf("Value: %i\n", evaluate(grid));
#endif
    /* Main loop for a game */
    while (!game_over(grid)) {
        /* While the move does not lead to a change on the grid */
        do {
#if IA
#if RANDOM
            dir = random_move();
#else
            dir = best_move(grid);
#endif
#else
            dir = ask_dir();
#endif
        /* Trying to play the move */
        } while (!move(grid, dir));
        /* Add a new tile at the end of each round */
        add_tile(grid);
        /* Display the new grid */
        display_grid(grid);
#if DEBUG
        printf("Value: %i\n", evaluate(grid));
#endif
    }
#endif
    return 0;
}
