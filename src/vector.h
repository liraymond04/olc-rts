#ifndef VECTOR_H
#define VECTOR_H

/*
 * Header macro definitions
 *
 * Macro should be added header files
 */

#define VECTOR_T(T)                                                            \
    struct vector_##T {                                                        \
        T *items;                                                              \
        int capacity;                                                          \
        int total;                                                             \
    };                                                                         \
    typedef struct vector_##T vector_##T##_t;

#define VECTOR_NEW(T) vector_##T##_t *vector_##T##_new(int init_capacity);

#define VECTOR_FREE(T) void vector_##T##_free(vector_##T##_t *vector);

#define VECTOR_CLEAR(T)                                                        \
    void vector_##T##_clear(vector_##T##_t *vector, int init_capacity);

#define VECTOR_AT(T) T vector_##T##_at(vector_##T##_t *vector, int i);

#define VECTOR_SET(T)                                                          \
    void vector_##T##_set(vector_##T##_t *vector, int i, T val);

#define VECTOR(T)                                                              \
    VECTOR_T(T)                                                                \
    VECTOR_NEW(T)                                                              \
    VECTOR_FREE(T)                                                             \
    VECTOR_CLEAR(T)                                                            \
    VECTOR_AT(T)                                                               \
    VECTOR_CLEAR(T)

/*
 * Source macro implementations
 *
 * Macro should be added to source files
 */

#define VECTOR_NEW_IMP(T)                                                      \
    vector_##T##_t *vector_##T##_new(int init_capacity) {                      \
        vector_##T##_t *vector =                                               \
            (vector_##T##_t *)malloc(sizeof(vector_##T##_t));                  \
        vector->items = (T *)malloc(sizeof(T) * init_capacity);                \
        vector->capacity = init_capacity;                                      \
        vector->total = 0;                                                     \
        return vector;                                                         \
    }

#define VECTOR_FREE_IMP(T)                                                     \
    void vector_##T##_free(vector_##T##_t *vector) {                           \
        free((void *)vector);                                                  \
        vector = NULL;                                                         \
    }

#define VECTOR_CLEAR_IMP(T)                                                    \
    void vector_##T##_clear(vector_##T##_t *vector, int init_capacity) {       \
        free((void *)vector);                                                  \
        vector = vector_##T##_new(init_capacity);                              \
    }

#define VECTOR_AT_IMP(T)                                                       \
    T vector_##T##_at(vector_##T##_t *vector, int i) {                         \
        return vector->items[i];                                               \
    }

#define VECTOR_SET_IMP(T)                                                      \
    void vector_##T##_set(vector_##T##_t *vector, int i, T val) {              \
        vector->items[i] = val;                                                \
    }

#define VECTOR_IMP(T)                                                          \
    VECTOR_NEW_IMP(T)                                                          \
    VECTOR_FREE_IMP(T)                                                         \
    VECTOR_CLEAR_IMP(T)                                                        \
    VECTOR_AT_IMP(T)                                                           \
    VECTOR_CLEAR_IMP(T)

#endif // !VECTOR_H
