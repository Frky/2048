#include "ui.h"
#include <stdio.h>


/*
 *  Display a line of '-' between two lines of 
 *  numbers
 */
void print_line_break(void) {
    int i;
    printf(" ");
    for (i = 0; i < N; i++) {
        printf("--- ");
    }
    printf("\n");
    return;
}

/*
 *  Display the grid on standard output with ascii chars 
 */
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

/*
 *  Ask a direction to the user as a char, 
 *  and return the corresponding dir
 */
int ask_dir(void) {
    char mov = '!';
    int dir;
    /* Getting user input */
    while (mov != 'z' && mov != 's' && mov != 'q' && mov != 'd') {
        scanf(" %c", &mov);
    }
    /* Parsing the input */
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
    /* Returning the corresponding direction */
    return dir;
}
