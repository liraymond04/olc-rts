#ifndef HEX_H
#define HEX_H

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int q, r, s;
} hex_t;

hex_t *hex_new(int q, int r);

bool hex_equals(hex_t *a, hex_t *b);

hex_t *hex_add(hex_t *a, hex_t *b);
hex_t *hex_subtract(hex_t *a, hex_t *b);
hex_t *hex_multiply(hex_t *a, hex_t *b);
void hex_add_v(hex_t *a, hex_t *b, hex_t *t);
void hex_subtract_v(hex_t *a, hex_t *b, hex_t *t);
void hex_multiply_v(hex_t *a, hex_t *b, hex_t *t);

hex_t *hex_scale(hex_t *hex, int k);
hex_t *hex_rotate_l(hex_t *hex);
hex_t *hex_rotate_r(hex_t *hex);
void hex_scale_v(hex_t *hex, int k, hex_t *t);
void hex_rotate_l_v(hex_t *hex, hex_t *t);
void hex_rotate_r_v(hex_t *hex, hex_t *t);

hex_t *hex_neighbor(hex_t *hex, int direction);
hex_t *hex_diagonal(hex_t *hex, int direction);
void hex_neighbor_v(hex_t *hex, int direction, hex_t *t);
void hex_diagonal_v(hex_t *hex, int direction, hex_t *t);

int hex_length(hex_t *hex);
int hex_distance(hex_t *a, hex_t *b);

typedef struct {
    double q, r, s;
} fractional_hex_t;

fractional_hex_t *fractional_hex_new(double q, double r);

#endif // !HEX_H
