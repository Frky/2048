#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "src/game.h"
#include "src/ui.h"
#include "src/ia.h"


#define     IA      1
#define     RANDOM  0
#define     DEBUG   1

int main(void) {
    int dir;
    srand(time(NULL));
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
    return 0;
}
