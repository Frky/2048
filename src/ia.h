#ifndef IA_H__
#define IA_H__

#include <stdint.h>

int evaluate(uint16_t **grid);

int random_move(void);

int best_move(uint16_t **grid);
int best_move_bin(grid_t grid, uint8_t nb_empty_box);

#endif
