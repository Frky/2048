#include <time.h>
#include <stdio.h>
#include "src/game.h"

int main(void) {
    int dir;
    char mov = '!';
    srand(time(NULL));
    /* Creation of a grid */
    uint16_t **grid = create_empty_grid();
    /* Initialisation of the grid */
    init_grid(grid);
    /* Display the grid */
    display_grid(grid);
    while (1) {
        mov = '!';
        while (mov != 'z' && mov != 's' && mov != 'q' && mov != 'd') {
            scanf(" %c", &mov);
        }
        switch (mov) {
        case 'z':
            dir = NORTH;
            break;
        case 's':
            dir = SOUTH;
            break;
        case 'q':
            dir = WEST;
            break;
        case 'd':
            dir = EAST;
            break;
        }
        move(grid, dir);
        add_tile(grid);
        display_grid(grid);
    }
    return 0;
}
