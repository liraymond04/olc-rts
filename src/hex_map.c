#include "hex_map.h"

HEX_MAP(int)

#define HEX_MAP_INIT_IMP(T)                                                    \
    hex_map_##T##_t *hex_map_##T##_init(int left, int top, int right,          \
                                        int bottom, T init_val, T null_val) {  \
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

HEX_MAP_INIT_IMP(int)
HEX_MAP_CLEAR_IMP(int)
HEX_MAP_AT_IMP(int)
HEX_MAP_SET_IMP(int)
