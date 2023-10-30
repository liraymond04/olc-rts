#include "rts.h"
#include "hex_grid.h"
#include "render.h"
#include "render_action/render_action.h"

double q, r;

rts_t *rts_new() {
    rts_t *rts = (rts_t *)malloc(sizeof(rts_t));
    rts->hex_grid = hex_grid_new(5, 5, 25);
    rts->camera_speed = 5.0f;

    rts->rq = render_queue_new();

    return rts;
}

void rts_free(rts_t *rts) {
    hex_grid_free(rts->hex_grid);
    render_queue_free(rts->rq);
    free(rts);
    rts = NULL;
}

void rts_input(rts_t *rts) {
    if (IsKeyDown(KEY_W)) {
        rts->hex_grid->translate_y -= rts->camera_speed;
    }
    if (IsKeyDown(KEY_S)) {
        rts->hex_grid->translate_y += rts->camera_speed;
    }
    if (IsKeyDown(KEY_A)) {
        rts->hex_grid->translate_x -= rts->camera_speed;
    }
    if (IsKeyDown(KEY_D)) {
        rts->hex_grid->translate_x += rts->camera_speed;
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
    render_queue_clear(rts->rq);

    hex_grid_draw(rts->hex_grid);
    if (hex_map_int_at(rts->hex_grid->_weights, q, r) !=
        rts->hex_grid->_weights->null_val) {
        render_queue_enqueue(rts->rq,
                             (render_action_t *)ra_draw_hex_new(
                                 rts->hex_grid, q, r, rts->hex_grid->_size, RED,
                                 (Color){ 0, 0, 0, 0 },
                                 hex_map_int_at(rts->hex_grid->_heights, q, r),
                                 (Color){ 0, 0, 0, 0 }));
    }

    // Run through render queue
    render_queue_node_t *cur = rts->rq->head;
    while (cur != NULL) {
        render_action_execute(cur->action);
        cur = cur->next;
    }
}
