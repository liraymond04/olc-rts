#ifndef RENDER_ACTION
#define RENDER_ACTION

#include "../hex_grid.h"

typedef struct render_action {
    void (*execute)(struct render_action *self);
    void (*free)(struct render_action *self);
    int id;
} render_action_t;

void render_action_execute(render_action_t *self);
void render_action_free(render_action_t *self);

/* Concrete render actions */
//------------------------------------------------------------------------------------
typedef struct {
    render_action_t base;
    hex_grid_t *hex_grid;
    int q;
    int r;
    double side_length;
    Color outline;
    Color fill;
    double height;
    Color side;
} ra_draw_hex_t;

void ra_draw_hex_execute(render_action_t *action);
void ra_draw_hex_free(render_action_t *action);

ra_draw_hex_t *ra_draw_hex_new(hex_grid_t *hex_grid, int q, int r,
                               double side_length, Color outline, Color fill,
                               double height, Color side);
//------------------------------------------------------------------------------------

#endif // !RENDER_ACTION
