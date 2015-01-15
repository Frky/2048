#ifndef IA_H__
#define IA_H__

#include <stdint.h>

int evaluate(uint16_t **grid);

int random_move(void);

int best_move(uint16_t **grid);

#endif
