#ifndef GAME_H__
#define GAME_H__

#include <stdbool.h>
#include <stdint.h>
#include "grid.h"

bool move(uint16_t **grid, uint8_t dir);

void add_tile(uint16_t **grid);

bool game_over(uint16_t **grid);

#endif
