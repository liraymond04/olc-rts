#ifndef HEX_MAP_H
#define HEX_MAP_H

#include <stdio.h>
#include <stdlib.h>

#define HEX_MAP_T(T)                                                           \
    struct hex_map_##T {                                                       \
        T **map;                                                               \
        int left_, top_;                                                       \
        int right_, bottom_;                                                   \
        T null_val;                                                            \
    };                                                                         \
    typedef struct hex_map_##T hex_map_##T##_t;

#define HEX_MAP_INIT(T)                                                        \
    hex_map_##T##_t *hex_map_##T##_init(int left, int top, int right,          \
                                        int bottom, T init_val, T null_val);

#define HEX_MAP_CLEAR(T)                                                       \
    void hex_map_##T##_clear(hex_map_##T##_t *hex_map, T val);

#define HEX_MAP_AT(T)                                                          \
    T hex_map_##T##_at(hex_map_##T##_t *hex_map, int q, int r);

#define HEX_MAP_SET(T)                                                         \
    void hex_map_##T##_set(hex_map_##T##_t *hex_map, int q, int r, T val);

#define HEX_MAP(T)   /* Define hex_map for int */                              \
    HEX_MAP_T(T)     /* Define type for int */                                 \
    HEX_MAP_INIT(T)  /* Initialize function for int */                         \
    HEX_MAP_CLEAR(T) /* Clear function for int  */                             \
    HEX_MAP_AT(T)    /* At function for int */                                 \
    HEX_MAP_SET(T)   /* Set function for int */

#endif // !HEX_MAP_H
