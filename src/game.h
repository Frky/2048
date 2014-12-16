#ifndef GAME_H__
#define GAME_H__

#include <stdlib.h>
#include <stdint.h>

#define     DEBUG   0
#define     N       4
#define     NORTH   0
#define     EAST    1
#define     SOUTH   2
#define     WEST    3

uint16_t **create_empty_grid(void);
    
void init_grid(uint16_t **grid);

void display_grid(uint16_t **grid);

int move(uint16_t **grid, uint8_t dir);

void add_tile(uint16_t **grid);

#endif
