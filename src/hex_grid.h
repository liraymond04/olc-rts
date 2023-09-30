#ifndef HEX_GRID_H
#define HEX_GRID_H

#include "raylib.h"
#include "hex_map.h"

HEX_MAP(int)

typedef struct {
    int width, height;
    double _size;

    hex_map_int_t *_heights;
    hex_map_int_t *_weights;

    double scale;
    double rotation_deg;
    double shear_x;
    double shear_y;
    double squeeze_x;
    double squeeze_y;
    double translate_x;
    double translate_y;
} hex_grid_t;

hex_grid_t *hex_grid_new(int width, int height, double _size);

void hex_grid_free(hex_grid_t *hex_grid);

void hex_grid_draw_hex(hex_grid_t *hex_grid, int q, int r, double side_length,
                       Color outline, Color fill, double height, Color side);
void hex_grid_draw(hex_grid_t *hex_grid);

void axial_round(double *q, double *r);

void ConvertToCartesian(hex_grid_t *hex_grid, double *x, double *y);
void ConvertToIsometric(hex_grid_t *hex_grid, double *x, double *y);
void CalculateIsometricHexagonVertices(hex_grid_t *hex_grid, double center_x,
                                       double center_y, double side_length,
                                       double vertices_x[],
                                       double vertices_y[]);
void CalculateHexagonCenter(hex_grid_t *hex_grid, int q, int r, double size,
                            double *center_x, double *center_y);
void CalculateAxialCoordinates(hex_grid_t *hex_grid, double center_x,
                               double center_y, double size, double *q,
                               double *r);
void CalculateIsometricAxialCoordinates(hex_grid_t *hex_grid, double center_x,
                                        double center_y, double size, double *q,
                                        double *r);

#endif // !HEX_GRID_H
