#include "rts.h"
#include "hex_grid.h"

double q, r;

rts_t *rts_new() {
    rts_t *rts = (rts_t *)malloc(sizeof(rts_t));
    rts->hex_grid = hex_grid_new(5, 5, 25);
    return rts;
}

void rts_free(rts_t *rts) {
    hex_grid_free(rts->hex_grid);
    free(rts);
    rts = NULL;
}

void rts_input(rts_t *rts) {
    double speed = 5.0f;

    if (IsKeyDown(KEY_W)) {
        rts->hex_grid->translate_y -= speed;
    }
    if (IsKeyDown(KEY_S)) {
        rts->hex_grid->translate_y += speed;
    }
    if (IsKeyDown(KEY_A)) {
        rts->hex_grid->translate_x -= speed;
    }
    if (IsKeyDown(KEY_D)) {
        rts->hex_grid->translate_x += speed;
    }
}

void rts_update(rts_t *rts) {
    q = 0;
    r = 0;
    CalculateIsometricAxialCoordinates(rts->hex_grid, rts->mouse_pos.x,
                                       rts->mouse_pos.y, rts->hex_grid->_size,
                                       &q, &r);
}

void rts_draw(rts_t *rts) {
    hex_grid_draw(rts->hex_grid);
    if (hex_map_int_at(rts->hex_grid->_weights, q, r) !=
        rts->hex_grid->_weights->null_val) {
        hex_grid_draw_hex(rts->hex_grid, q, r, rts->hex_grid->_size, RED,
                          (Color){ 0, 0, 0, 0 },
                          hex_map_int_at(rts->hex_grid->_heights, q, r),
                          (Color){ 0, 0, 0, 0 });
    }
}
