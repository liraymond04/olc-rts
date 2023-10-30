#include "render_action.h"

void ra_draw_hex_execute(render_action_t *action) {
    ra_draw_hex_t *draw_hex = (ra_draw_hex_t *)action;
    hex_grid_draw_hex(draw_hex->hex_grid, draw_hex->q, draw_hex->r,
                      draw_hex->side_length, draw_hex->outline, draw_hex->fill,
                      draw_hex->height, draw_hex->side);
}

void ra_draw_hex_free(render_action_t *action) {
    free(action);
    action = NULL;
}

ra_draw_hex_t *ra_draw_hex_new(hex_grid_t *hex_grid, int q, int r,
                               double side_length, Color outline, Color fill,
                               double height, Color side) {

    ra_draw_hex_t *action = (ra_draw_hex_t *)malloc(sizeof(ra_draw_hex_t));
    action->base.execute = ra_draw_hex_execute;
    action->base.free = ra_draw_hex_free;
    action->hex_grid = hex_grid;
    action->q = q;
    action->r = r;
    action->side_length = side_length;
    action->outline = outline;
    action->fill = fill;
    action->height = height;
    action->side = side;
    return action;
}
