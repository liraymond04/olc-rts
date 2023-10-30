#include "render.h"
#include "render_action/render_action.h"

render_queue_node_t *render_queue_node_new(render_action_t *action) {
    render_queue_node_t *new_node = (render_queue_node_t *)malloc(sizeof(render_queue_node_t));
    new_node->action = action;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

void render_queue_node_free(render_queue_node_t *node) {
    render_action_free(node->action);
    free(node);
    node = NULL;
}

render_queue_t *render_queue_new() {
    render_queue_t *rq = (render_queue_t *)malloc(sizeof(render_queue_t));
    rq->head = NULL;
    rq->tail = NULL;
    rq->length = 0;
    return rq;
}

void render_queue_free(render_queue_t *rq) {
    render_queue_node_t *tmp;
    while (rq->head != NULL) {
        tmp = rq->head;
        rq->head = rq->head->next;
        render_queue_node_free(tmp);
        tmp = NULL;
        if (rq->head == rq->tail) {
            rq->tail = NULL;
        }
    }
    rq->length = 0;
}

void render_queue_clear(render_queue_t *rq) {
    render_queue_free(rq);
    rq = render_queue_new();
}

void render_queue_enqueue(render_queue_t *rq, render_action_t *action) {
    render_queue_node_t *new_node = render_queue_node_new(action);
    new_node->prev = rq->tail;

    if (rq->tail) {
        rq->tail->next = new_node;
    }
    rq->tail = new_node;

    if (rq->head == NULL) {
        rq->head = new_node;
    }
}
