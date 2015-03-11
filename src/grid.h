#ifndef GRID_H__
#define GRID_H__

#include <stdint.h>

#define DEBUG_BIN 0

#define     N       4
#define     NORTH   0
#define     EAST    1
#define     SOUTH   2
#define     WEST    3

typedef unsigned __int128 grid_t;
typedef unsigned __int128 uint128_tt;
typedef uint32_t row_t;

extern const uint128_tt BOX_MASK;
extern const uint128_tt BOX_SIZE;

uint16_t **create_empty_grid(void);
    
void init_grid(uint16_t **grid);

uint16_t **copy_grid(uint16_t **grid);

uint16_t max_tile(uint16_t **grid);
uint8_t max_tile_bin(grid_t grid);

void free_grid(uint16_t **grid);

grid_t create_empty_grid_bin(void);
uint8_t get_box(grid_t grid, uint8_t n);
grid_t set_box(grid_t grid, uint32_t n, uint8_t value);
grid_t init_grid_bin(grid_t grid, uint8_t *nb_empty_box_out);
uint32_t bin_to_tile(uint8_t box);
row_t get_row(grid_t grid, uint8_t dir, uint8_t row_id);
grid_t set_row(grid_t grid, uint8_t dir, uint8_t row_id, uint32_t row);
row_t set_box_in_row(row_t row, uint8_t n, uint8_t val);
uint8_t get_box_in_row(row_t row, uint8_t n);

#endif
