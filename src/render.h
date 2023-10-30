#ifndef RENDER_H
#define RENDER_H

// TODO
// - render layer use vector imp
#include "vector.h"
#include <stdlib.h>
#include "stdio.h"

#include "render_action/render_action.h"

//------------------------------------------------------------------------------------
// Render queue
//------------------------------------------------------------------------------------
typedef struct render_queue_node {
    render_action_t *action;
    struct render_queue_node *next;
    struct render_queue_node *prev;
} render_queue_node_t;

render_queue_node_t *render_queue_node_new(render_action_t *action);

void render_queue_node_free(render_queue_node_t *node);

typedef struct {
    render_queue_node_t *head;
    render_queue_node_t *tail;
    int length;
} render_queue_t;

render_queue_t *render_queue_new();

void render_queue_free(render_queue_t *rq);

void render_queue_clear(render_queue_t *rq);

void render_queue_enqueue(render_queue_t *rq, render_action_t *action);
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Render layer
//------------------------------------------------------------------------------------
VECTOR(render_queue_t);

typedef struct {
    vector_render_queue_t_t *items;
} render_layer_t;

render_layer_t *render_layer_new();

void render_layer_free(render_layer_t *rl);

void render_layer_add_at(render_layer_t *rl, int i);
void render_layer_remove_at(render_layer_t *rl, int i);

render_queue_t *render_layer_at(render_layer_t *rl, int i);

void render_layer_clear(render_layer_t *rl);
//------------------------------------------------------------------------------------

#endif // !RENDER_H
