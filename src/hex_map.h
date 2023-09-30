#ifndef HEX_MAP_H
#define HEX_MAP_H

#include <stdio.h>
#include <stdlib.h>

/* 
 * Header macro definitions
 * 
 * Macro should be added header files
 * ex) hex_grid.h
 */

#define HEX_MAP_T(T)                                                           \
    struct hex_map_##T {                                                       \
        T **map;                                                               \
        int left_, top_;                                                       \
        int right_, bottom_;                                                   \
        T null_val;                                                            \
    };                                                                         \
    typedef struct hex_map_##T hex_map_##T##_t;

#define HEX_MAP_NEW(T)                                                         \
    hex_map_##T##_t *hex_map_##T##_new(int left, int top, int right,           \
                                       int bottom, T init_val, T null_val);

#define HEX_MAP_FREE(T) void hex_map_##T##_free(hex_map_##T##_t *hex_map);

#define HEX_MAP_CLEAR(T)                                                       \
    void hex_map_##T##_clear(hex_map_##T##_t *hex_map, T val);

#define HEX_MAP_AT(T)                                                          \
    T hex_map_##T##_at(hex_map_##T##_t *hex_map, int q, int r);

#define HEX_MAP_SET(T)                                                         \
    void hex_map_##T##_set(hex_map_##T##_t *hex_map, int q, int r, T val);

#define HEX_MAP(T)   /* Define hex_map for int */                              \
    HEX_MAP_T(T)     /* Define type for int */                                 \
    HEX_MAP_NEW(T)   /* Initialize function for int */                         \
    HEX_MAP_FREE(T)  /* Free function for int*/                                \
    HEX_MAP_CLEAR(T) /* Clear function for int  */                             \
    HEX_MAP_AT(T)    /* At function for int */                                 \
    HEX_MAP_SET(T)   /* Set function for int */

/* 
 * Source macro implementations
 * 
 * Macro should be added to source files
 * ex) hex_grid.c
 */

#define HEX_MAP_NEW_IMP(T)                                                     \
    hex_map_##T##_t *hex_map_##T##_new(int left, int top, int right,           \
                                       int bottom, T init_val, T null_val) {   \
        hex_map_##T##_t *hex_map =                                             \
            (hex_map_##T##_t *)malloc(sizeof(hex_map_##T##_t));                \
        if (hex_map == NULL) {                                                 \
        }                                                                      \
        hex_map->left_ = left;                                                 \
        hex_map->top_ = top;                                                   \
        hex_map->right_ = right;                                               \
        hex_map->bottom_ = bottom;                                             \
        hex_map->null_val = null_val;                                          \
        int height = bottom - top + 1;                                         \
        int width = right - left + 1;                                          \
        hex_map->map = (T **)malloc(sizeof(T *) * height);                     \
        if (hex_map->map == NULL) {                                            \
        }                                                                      \
        for (int r = 0; r < height; r++) {                                     \
            hex_map->map[r] = (T *)malloc(sizeof(T) * width);                  \
            if (hex_map->map[r] == NULL) {                                     \
            }                                                                  \
            for (int c = 0; c < width; c++) {                                  \
                hex_map->map[r][c] = init_val;                                 \
            }                                                                  \
        }                                                                      \
        return hex_map;                                                        \
    }

#define HEX_MAP_FREE_IMP(T)                                                    \
    void hex_map_##T##_free(hex_map_##T##_t *hex_map) {                        \
        int height = hex_map->bottom_ - hex_map->top_ + 1;                     \
        for (int r = 0; r < height; r++) {                                     \
            free(hex_map->map[r]);                                             \
        }                                                                      \
        free(hex_map->map);                                                    \
        free(hex_map);                                                         \
        hex_map = NULL;                                                        \
    }

#define HEX_MAP_CLEAR_IMP(T)                                                   \
    void hex_map_##T##_clear(hex_map_##T##_t *hex_map, T val) {                \
        int height = hex_map->bottom_ - hex_map->top_ + 1;                     \
        int width = hex_map->right_ - hex_map->left_ + 1;                      \
        for (int r = 0; r < height; r++) {                                     \
            for (int c = 0; c < width; c++) {                                  \
                hex_map->map[r][c] = val;                                      \
            }                                                                  \
        }                                                                      \
    }

#define HEX_MAP_AT_IMP(T)                                                      \
    T hex_map_##T##_at(hex_map_##T##_t *hex_map, int q, int r) {               \
        int j = r - hex_map->top_;                                             \
        int i = q - hex_map->left_ + (r >> 1);                                 \
        if (j < 0 || j >= (hex_map->bottom_ - hex_map->top_ + 1) || i < 0 ||   \
            i >= (hex_map->right_ - hex_map->left_ + 1)) {                     \
            return hex_map->null_val;                                          \
        }                                                                      \
        return hex_map->map[j][i];                                             \
    }

#define HEX_MAP_SET_IMP(T)                                                     \
    void hex_map_##T##_set(hex_map_##T##_t *hex_map, int q, int r, T val) {    \
        int j = r - hex_map->top_;                                             \
        int i = q - hex_map->left_ + (r >> 1);                                 \
        if (j < 0 || j >= (hex_map->bottom_ - hex_map->top_ + 1) || i < 0 ||   \
            i >= (hex_map->right_ - hex_map->left_ + 1)) {                     \
            return;                                                            \
        }                                                                      \
        hex_map->map[j][i] = val;                                              \
    }

#define HEX_MAP_IMP(T)   /* Define hex_map for int */                          \
    HEX_MAP_NEW_IMP(T)   /* Initialize function for int */                     \
    HEX_MAP_FREE_IMP(T)  /* Free function for int*/                            \
    HEX_MAP_CLEAR_IMP(T) /* Clear function for int  */                         \
    HEX_MAP_AT_IMP(T)    /* At function for int */                             \
    HEX_MAP_SET_IMP(T)   /* Set function for int */

#endif // !HEX_MAP_H
