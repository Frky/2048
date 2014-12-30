#ifndef GRID_H__
#define GRID_H__

#include <stdint.h>

#define     N       4
#define     NORTH   0
#define     EAST    1
#define     SOUTH   2
#define     WEST    3

uint16_t **create_empty_grid(void);
    
void init_grid(uint16_t **grid);

uint16_t **copy_grid(uint16_t **grid);

uint16_t max_tile(uint16_t **grid);

void free_grid(uint16_t **grid);

#endif
