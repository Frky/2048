#ifndef GAME_H__
#define GAME_H__

#include <stdbool.h>
#include <stdint.h>
#include "grid.h"

bool move(uint16_t **grid, uint8_t dir);

void add_tile(uint16_t **grid);

bool game_over(uint16_t **grid);

grid_t move_bin(grid_t grid, uint8_t dir, uint8_t *nb_merges_out);
grid_t add_tile_bin(grid_t grid);
bool merge_possible_bin(grid_t grid);

#endif
