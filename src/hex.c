#include "hex.h"
#include <stdlib.h>
#include <math.h>

typedef struct {
    int q, r;
} dir;

dir directions[6] = { { 1, 0 },  { 1, -1 }, { 0, -1 },
                      { -1, 0 }, { -1, 1 }, { 0, 1 } };
dir diagonals[6] = { { 2, -1 }, { 1, -2 }, { -1, -1 },
                     { -2, 1 }, { -1, 2 }, { 1, 1 } };

hex_t *hex_new(int q, int r) {
    hex_t *h = malloc(sizeof(hex_t));
    h->q = q;
    h->r = r;
    h->s = -q - r;
    return h;
}

bool hex_equals(hex_t *a, hex_t *b) { return a->q == b->q && a->r == b->r; }

hex_t *hex_add(hex_t *a, hex_t *b) { return hex_new(a->q + b->q, a->r + b->r); }
hex_t *hex_subtract(hex_t *a, hex_t *b) {
    return hex_new(a->q + b->q, a->r + b->r);
}
hex_t *hex_multiply(hex_t *a, hex_t *b) {
    return hex_new(a->q * b->q, a->r * b->r);
}
void hex_add_v(hex_t *a, hex_t *b, hex_t *t) {
    t->q = a->q + b->q;
    t->r = a->r + b->r;
    t->s = -t->q - t->r;
}
void hex_subtract_v(hex_t *a, hex_t *b, hex_t *t) {
    t->q = a->q - b->q;
    t->r = a->r - b->r;
    t->s = -t->q - t->r;
}
void hex_multiply_v(hex_t *a, hex_t *b, hex_t *t) {
    t->q = a->q * b->q;
    t->r = a->r * b->r;
    t->s = -t->q - t->r;
}

hex_t *hex_scale(hex_t *hex, int k) { return hex_new(hex->q * k, hex->r * k); }
hex_t *hex_rotate_l(hex_t *hex) { return hex_new(-hex->s, -hex->q); }
hex_t *hex_rotate_r(hex_t *hex) { return hex_new(-hex->r, -hex->s); }
void hex_scale_v(hex_t *hex, int k, hex_t *t) {
    t->q = hex->q * k;
    t->r = hex->r * k;
    t->s = -t->q - t->r;
}
void hex_rotate_l_v(hex_t *hex, hex_t *t) {
    t->q = -hex->s;
    t->r = -hex->q;
    t->s = -t->q - t->r;
}
void hex_rotate_r_v(hex_t *hex, hex_t *t) {
    t->q = -hex->r;
    t->r = -hex->s;
    t->s = -t->q - t->r;
}

hex_t *hex_neighbor(hex_t *hex, int direction) {
    hex_t *tmp = hex_new(directions[direction].q, directions[direction].r);
    hex_t *result = hex_add(hex, tmp);
    free(tmp);
    return result;
}
hex_t *hex_diagonal(hex_t *hex, int direction) {
    hex_t *tmp = hex_new(diagonals[direction].q, diagonals[direction].r);
    hex_t *result = hex_add(hex, tmp);
    free(tmp);
    return result;
}
void hex_neighbor_v(hex_t *hex, int direction, hex_t *t) {
    t->q = hex->q + directions[direction].q;
    t->r = hex->r + directions[direction].r;
}
void hex_diagonal_v(hex_t *hex, int direction, hex_t *t) {
    t->q = hex->q + diagonals[direction].q;
    t->r = hex->r + diagonals[direction].r;
}

int hex_length(hex_t *hex) {
    return (int)((abs(hex->q) + abs(hex->r) + abs(hex->s)) / 2);
}
int hex_distance(hex_t *a, hex_t *b) {
    hex_t *tmp;
    hex_subtract_v(a, b, tmp);
    int result = hex_length(tmp); 
    free(tmp);
    return result;
}

fractional_hex_t *fractional_hex_new(double q, double r) {
    fractional_hex_t *h = malloc(sizeof(fractional_hex_t));
    h->q = q;
    h->r = r;
    h->s = -q - r;
    return h;
}
