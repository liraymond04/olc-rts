#include "hex.h"
#include "hex_grid.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>

HEX_MAP_IMP(int);

bool color_equals(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.b;
}

hex_grid_t *hex_grid_new(int width, int height, double _size) {
    hex_grid_t *h = malloc(sizeof(hex_grid_t));
    h->_heights = hex_map_int_new(0, 0, width - 1, height - 1, 10, -1);
    h->_weights = hex_map_int_new(0, 0, width - 1, height - 1, 1, -1);
    h->width = width;
    h->height = width;
    h->_size = _size;

    h->scale = 1.0;
    h->rotation_deg = 10.0;
    h->shear_x = 0.0;
    h->shear_y = 0.0;
    h->squeeze_x = 1.0;
    h->squeeze_y = 0.5;
    h->translate_x = 0.0f;
    h->translate_y = 0.0f;
    return h;
}

void hex_grid_free(hex_grid_t *hex_grid) {
    hex_map_int_free(hex_grid->_heights);
    hex_map_int_free(hex_grid->_weights);
    free(hex_grid);
    hex_grid = NULL;
}

void hex_grid_draw_hex(hex_grid_t *hex_grid, int q, int r, double side_length,
                       Color outline, Color fill, double height, Color side) {
    bool debug = false;
    double vertices_x[6];
    double vertices_y[6];

    double center_x, center_y;

    // Calculate the vertices of the isometric hexagon
    CalculateHexagonCenter(hex_grid, q, r, side_length, &center_x, &center_y);

    // Cull if out of screen bounds
    double checkX = center_x, checkY = center_y;
    ConvertToIsometric(hex_grid, &checkX, &checkY);
    if (checkX - side_length > GetScreenWidth() || checkX + side_length < 0 ||
        checkY - height - side_length > GetScreenHeight() ||
        checkY + side_length < 0) {
        return;
    }

    CalculateIsometricHexagonVertices(hex_grid, center_x, center_y, side_length,
                                      vertices_x, vertices_y);

    // Draw back lines between the vertices to form the hexagon
    for (int i = 2; i < 6; i++) {
        int j = (i + 1) % 6; // Index of the next vertex with wrapping
        DrawLine(vertices_x[i], vertices_y[i], vertices_x[j], vertices_y[j],
                 outline);
    }

    // Draw back lines connecting bottom and top hexagons
    if (height > 0) {
        for (int i = 3; i < 6; i++) {
            DrawLine(vertices_x[i], vertices_y[i], vertices_x[i],
                     vertices_y[i] - height, outline);
        }
    }

    Color null_color = { 0, 0, 0, 0 };
    // Colour side faces
    if (!color_equals(side, null_color) && height > 0) {
        DrawTriangle(
            (Vector2){ (int)vertices_x[1], (int)(vertices_y[1]) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0]) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            debug ? RED : side);
        DrawTriangle(
            (Vector2){ (int)vertices_x[1], (int)(vertices_y[1]) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            (Vector2){ (int)vertices_x[1], (int)(vertices_y[1] - height) },
            debug ? BLUE : side);
        DrawTriangle(
            (Vector2){ (int)vertices_x[2], (int)(vertices_y[2]) },
            (Vector2){ (int)vertices_x[1], (int)(vertices_y[1]) },
            (Vector2){ (int)vertices_x[1], (int)(vertices_y[1] - height) },
            debug ? GREEN : side);
        DrawTriangle(
            (Vector2){ (int)vertices_x[2], (int)(vertices_y[2]) },
            (Vector2){ (int)vertices_x[1], (int)(vertices_y[1] - height) },
            (Vector2){ (int)vertices_x[2], (int)(vertices_y[2] - height) },
            debug ? YELLOW : side);
        DrawTriangle(
            (Vector2){ (int)vertices_x[5], (int)(vertices_y[5]) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0]) },
            debug ? MAGENTA : side);
        DrawTriangle(
            (Vector2){ (int)vertices_x[5], (int)(vertices_y[5]) },
            (Vector2){ (int)vertices_x[5], (int)(vertices_y[5] - height) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            debug ? GRAY : side);
    }

    // Draw front lines connecting bottom and top hexagons
    for (int i = 0; i < 2; i++) {
        int j = (i + 1) % 6; // Index of the next vertex with wrapping
        DrawLine(vertices_x[i], vertices_y[i], vertices_x[j], vertices_y[j],
                 outline);
    }
    DrawLine(vertices_x[0], vertices_y[0], vertices_x[5], vertices_y[5],
             outline);

    // Draw front lines connecting bottom and top hexagons
    if (height > 0) {
        for (int i = 0; i < 3; i++) {
            DrawLine(vertices_x[i], vertices_y[i], vertices_x[i],
                     vertices_y[i] - height, outline);
        }
    }
    DrawLine(vertices_x[5], vertices_y[5], vertices_x[5],
             vertices_y[5] - height, outline);

    // Colour top face
    if (!color_equals(fill, null_color)) {
        DrawTriangle(
            (Vector2){ (int)vertices_x[1], (int)(vertices_y[1] - height) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            (Vector2){ (int)vertices_x[2], (int)(vertices_y[2] - height) },
            debug ? RED : fill);
        DrawTriangle(
            (Vector2){ (int)vertices_x[2], (int)(vertices_y[2] - height) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            (Vector2){ (int)vertices_x[3], (int)(vertices_y[3] - height) },
            debug ? BLUE : fill);
        DrawTriangle(
            (Vector2){ (int)vertices_x[3], (int)(vertices_y[3] - height) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            (Vector2){ (int)vertices_x[4], (int)(vertices_y[4] - height) },
            debug ? GREEN : fill);
        DrawTriangle(
            (Vector2){ (int)vertices_x[4], (int)(vertices_y[4] - height) },
            (Vector2){ (int)vertices_x[0], (int)(vertices_y[0] - height) },
            (Vector2){ (int)vertices_x[5], (int)(vertices_y[5] - height) },
            debug ? YELLOW : fill);
    }

    // Draw top outline
    if (height > 0) {
        for (int i = 0; i < 6; i++) {
            int j = (i + 1) % 6; // Index of the next vertex with wrapping
            DrawLine(vertices_x[i], vertices_y[i] - height, vertices_x[j],
                     vertices_y[j] - height, outline);
        }
    }
}

void hex_grid_draw(hex_grid_t *hex_grid) {
    for (int i = 0; i < hex_grid->height; i++) {
        int r = i;
        for (int j = 0; j < hex_grid->width; j++) {
            int q = j - i / 2;
            Color col = DARKGREEN;
            Color side = { 139, 69, 19, 255 };
            // if (path.find(Hex(q, r)) != path.end()) {
            //     col = olc::YELLOW;
            // }
            // if (start != nullptr && start->q == q && start->r == r) {
            //     col = olc::GREEN;
            // }
            // if (end != nullptr && end->q == q && end->r == r) {
            //     col = olc::RED;
            // }
            hex_grid_draw_hex(hex_grid, q, r, hex_grid->_size, WHITE, col,
                              hex_map_int_at(hex_grid->_heights, q, r), side);
        }
    }
}

void axial_round(double *q, double *r) {
    int qgrid = round(*q), rgrid = round(*r);
    *q -= qgrid, *r -= rgrid; // remainder
    int dq = round((*q) + 0.5 * (*r)) * ((*r) * (*r) < (*q) * (*q));
    int dr = round((*r) + 0.5 * (*q)) * ((*r) * (*r) >= (*q) * (*q));
    *q = qgrid + dq;
    *r = rgrid + dr;
}

static hex_t hex_axial_round(fractional_hex_t hex) {
    double q = hex.q, r = hex.r;
    int qgrid = round(q), rgrid = round(r);
    q -= qgrid, r -= rgrid; // remainder
    int dq = round(q + 0.5 * r) * (r * r < q * q);
    int dr = round(r + 0.5 * q) * (r * r >= q * q);
    q = qgrid + dq;
    r = rgrid + dr;
    return (hex_t){ q, r, -q - r };
}

static fractional_hex_t hex_lerp(fractional_hex_t a, fractional_hex_t b,
                                 double t) {
    double q = a.q * (1.0 - t) + b.q * t;
    double r = a.r * (1.0 - t) + b.r * t;
    return (fractional_hex_t){ q, r, -q - r };
}

void ConvertToIsometric(hex_grid_t *hex_grid, double *x, double *y) {
    // Scale
    *x = (hex_grid->scale * (*x));
    *y = (hex_grid->scale * (*y));

    // Rotation
    double angle_rad = hex_grid->rotation_deg * M_PI / 180.0;
    double rotatedX = ((*x) * cos(angle_rad) - (*y) * sin(angle_rad));
    double rotatedY = ((*x) * sin(angle_rad) + (*y) * cos(angle_rad));
    *x = rotatedX;
    *y = rotatedY;

    // Shearing
    *x = ((*x) + hex_grid->shear_x * (*y));
    *y = ((*y) + hex_grid->shear_y * (*x));

    // Squeezing
    *x = ((*x) * hex_grid->squeeze_x);
    *y = ((*y) * hex_grid->squeeze_y);

    // Translation
    *x -= hex_grid->translate_x;
    *y -= hex_grid->translate_y;
}

// Function to apply an affine transformation to convert isometric coordinates
// to Cartesian coordinates
void ConvertToCartesian(hex_grid_t *hex_grid, double *x, double *y) {

    // Untranslation
    *x += hex_grid->translate_x;
    *y += hex_grid->translate_y;

    // Unsqueezing
    *x = ((*x) / hex_grid->squeeze_x);
    *y = ((*y) / hex_grid->squeeze_y);

    // Unshearing
    *x = ((*x) - hex_grid->shear_x * (*y));
    *y = ((*y) - hex_grid->shear_y * (*x));

    // Inverse rotation
    double angle_rad = (-hex_grid->rotation_deg) * M_PI / 180.0;
    double rotatedX = ((*x) * cos(angle_rad) - (*y) * sin(angle_rad));
    double rotatedY = ((*x) * sin(angle_rad) + (*y) * cos(angle_rad));
    *x = rotatedX;
    *y = rotatedY;

    // Inverse scaling
    *x = ((*x) / hex_grid->scale);
    *y = ((*y) / hex_grid->scale);
}

// Function to calculate the vertices of a perfect hexagon in isometric space
void CalculateIsometricHexagonVertices(hex_grid_t *hex_grid, double centerX,
                                       double centerY, double sideLength,
                                       double verticesX[], double verticesY[]) {
    double angle_deg = 30; // Start angle (in degrees)
    double angle_rad;

    for (int i = 0; i < 6; i++) {
        angle_rad = M_PI / 180 * angle_deg; // Convert angle to radians

        // Calculate Cartesian coordinates of the vertex
        double x = centerX + sideLength * cos(angle_rad);
        double y = centerY + sideLength * sin(angle_rad);

        // Convert Cartesian coordinates to isometric coordinates
        ConvertToIsometric(hex_grid, &x, &y);

        // Assign the transformed coordinates to the vertex arrays
        verticesX[i] = x;
        verticesY[i] = y;

        angle_deg += 60; // Increment angle by 60 degrees for the next vertex
    }
}

void CalculateHexagonCenter(hex_grid_t *hex_grid, int q, int r, double size,
                            double *centerX, double *centerY) {
    // Conversion formulas from axial to Cartesian coordinates
    *centerX = size * sqrt(3) * (q + r / 2.0);
    *centerY = size * 3 / 2.0 * r;
}

// Function to calculate the axial coordinates (q, r) of a hexagon from its
// center X and Y coordinates
void CalculateAxialCoordinates(hex_grid_t *hex_gri, double centerX,
                               double centerY, double size, double *q,
                               double *r) {
    // Conversion formulas from Cartesian to axial coordinates
    double qD = (centerX * sqrt(3) / 3.0 - centerY / 3.0) / size;
    double rD = centerY * 2.0 / 3.0 / size;
    *q = qD;
    *r = rD;
    axial_round(q, r);
}

void CalculateIsometricAxialCoordinates(hex_grid_t *hex_grid, double centerX,
                                        double centerY, double size, double *q,
                                        double *r) {
    ConvertToCartesian(hex_grid, &centerX, &centerY);
    CalculateAxialCoordinates(hex_grid, centerX, centerY, size, q, r);
}
