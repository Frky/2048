#ifndef UI_H__
#define UI_H__

#include "game.h"

void display_grid(uint16_t **grid);
void display_grid_bin(grid_t grid);

int ask_dir(void);

void display_row(row_t row);

#endif
