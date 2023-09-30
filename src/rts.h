#ifndef RTS_H
#define RTS_H

#include "hex_grid.h"

typedef struct {
    hex_grid_t *hex_grid;
    Vector2 mouse_pos;
} rts_t;

rts_t *rts_new();

void rts_free(rts_t *rts);

void rts_input(rts_t *rts);
void rts_update(rts_t *rts);
void rts_draw(rts_t *rts);

void rts_tick(rts_t *rts);

#endif // !RTS_H
