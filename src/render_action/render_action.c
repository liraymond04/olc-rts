#include "render_action.h"

void render_action_execute(render_action_t *self) {
    self->execute(self);
}

void render_action_free(render_action_t *self) {
    self->free(self);
}
