#include "Hex.h"
#include "Unit.h"
#include "IRender.h"

std::vector<Hex> Hex::directions = { Hex(1, 0),  Hex(1, -1), Hex(0, -1),
                                     Hex(-1, 0), Hex(-1, 1), Hex(0, 1) },
                 Hex::diagonals = { Hex(2, -1), Hex(1, -2), Hex(-1, -1),
                                    Hex(-2, 1), Hex(-1, 2), Hex(1, 1) };

static void axial_round(double &q, double &r) {
    int qgrid = round(q), rgrid = round(r);
    q -= qgrid, r -= rgrid; // remainder
    int dq = round(q + 0.5 * r) * (r * r < q * q);
    int dr = round(r + 0.5 * q) * (r * r >= q * q);
    q = qgrid + dq;
    r = rgrid + dr;
}

static Hex hex_axial_round(FractionalHex hex) {
    double q = hex.q, r = hex.r;
    int qgrid = round(q), rgrid = round(r);
    q -= qgrid, r -= rgrid; // remainder
    int dq = round(q + 0.5 * r) * (r * r < q * q);
    int dr = round(r + 0.5 * q) * (r * r >= q * q);
    q = qgrid + dq;
    r = rgrid + dr;
    return Hex(q, r);
}

static FractionalHex hex_lerp(FractionalHex a, FractionalHex b, double t) {
    return FractionalHex(a.q * (1.0 - t) + b.q * t, a.r * (1.0 - t) + b.r * t);
}

static std::vector<Hex> hex_linedraw(Hex a, Hex b) {
    int N = hex_distance(a, b);
    FractionalHex a_nudge = FractionalHex(a.q + 1e-06, a.r + 1e-06);
    FractionalHex b_nudge = FractionalHex(b.q + 1e-06, b.r + 1e-06);
    std::vector<Hex> results = {};
    double step = 1.0 / std::max(N, 1);
    for (int i = 0; i <= N; i++) {
        results.push_back(
            hex_axial_round(hex_lerp(a_nudge, b_nudge, step * i)));
    }
    return results;
}

void HexGrid::ConvertToIsometric(double &x, double &y) {
    // Scale
    x = (scale * x);
    y = (scale * y);

    // Rotation
    double angle_rad = rotation_deg * M_PI / 180.0;
    double rotatedX = (x * cos(angle_rad) - y * sin(angle_rad));
    double rotatedY = (x * sin(angle_rad) + y * cos(angle_rad));
    x = rotatedX;
    y = rotatedY;

    // Shearing
    x = (x + shear_x * y);
    y = (y + shear_y * x);

    // Squeezing
    x = (x * squeeze_x);
    y = (y * squeeze_y);

    // Translation
    x -= translate_x;
    y -= translate_y;
}

// Function to apply an affine transformation to convert isometric coordinates
// to Cartesian coordinates
void HexGrid::ConvertToCartesian(double &x, double &y) {

    // Untranslation
    x += translate_x;
    y += translate_y;

    // Unsqueezing
    x = (x / squeeze_x);
    y = (y / squeeze_y);

    // Unshearing
    x = (x - shear_x * y);
    y = (y - shear_y * x);

    // Inverse rotation
    double angle_rad = (-rotation_deg) * M_PI / 180.0;
    double rotatedX = (x * cos(angle_rad) - y * sin(angle_rad));
    double rotatedY = (x * sin(angle_rad) + y * cos(angle_rad));
    x = rotatedX;
    y = rotatedY;

    // Inverse scaling
    x = (x / scale);
    y = (y / scale);
}

// Function to calculate the vertices of a perfect hexagon in isometric space
void HexGrid::CalculateIsometricHexagonVertices(double centerX, double centerY,
                                                double sideLength,
                                                double verticesX[],
                                                double verticesY[]) {
    double angle_deg = 30; // Start angle (in degrees)
    double angle_rad;

    for (int i = 0; i < 6; i++) {
        angle_rad = M_PI / 180 * angle_deg; // Convert angle to radians

        // Calculate Cartesian coordinates of the vertex
        double x = centerX + sideLength * cos(angle_rad);
        double y = centerY + sideLength * sin(angle_rad);

        // Convert Cartesian coordinates to isometric coordinates
        ConvertToIsometric(x, y);

        // Assign the transformed coordinates to the vertex arrays
        verticesX[i] = x;
        verticesY[i] = y;

        angle_deg += 60; // Increment angle by 60 degrees for the next vertex
    }
}

void HexGrid::CalculateHexagonCenter(int q, int r, double size, double &centerX,
                                     double &centerY) {
    // Conversion formulas from axial to Cartesian coordinates
    centerX = size * sqrt(3) * (q + r / 2.0);
    centerY = size * 3 / 2.0 * r;
}

// Function to calculate the axial coordinates (q, r) of a hexagon from its
// center X and Y coordinates
void HexGrid::CalculateAxialCoordinates(double centerX, double centerY,
                                        double size, double &q, double &r) {
    // Conversion formulas from Cartesian to axial coordinates
    double qD = (centerX * sqrt(3) / 3.0 - centerY / 3.0) / size;
    double rD = centerY * 2.0 / 3.0 / size;
    q = qD;
    r = rD;
    axial_round(q, r);
}

void HexGrid::CalculateIsometricAxialCoordinates(double centerX, double centerY,
                                                 double size, double &q,
                                                 double &r) {
    ConvertToCartesian(centerX, centerY);
    CalculateAxialCoordinates(centerX, centerY, size, q, r);
}

void HexGrid::DrawHex(int q, int r, double sideLength, olc::Pixel outline,
                      olc::Pixel fill, double height, int *mask,
                      olc::Pixel side) {
    double verticesX[6];
    double verticesY[6];

    double centerX, centerY;

    // Calculate the vertices of the isometric hexagon
    CalculateHexagonCenter(q, r, sideLength, centerX, centerY);

    // Cull if out of screen bounds
    double checkX = centerX, checkY = centerY;
    ConvertToIsometric(checkX, checkY);
    if (checkX - sideLength > game->ScreenWidth() || checkX + sideLength < 0 ||
        checkY - height - sideLength > game->ScreenHeight() ||
        checkY + sideLength < 0) {
        return;
    }

    CalculateIsometricHexagonVertices(centerX, centerY, sideLength, verticesX,
                                      verticesY);

    // Draw back lines between the vertices to form the hexagon
    for (int i = 2; i < 6; i++) {
        int j = (i + 1) % 6; // Index of the next vertex with wrapping
        game->DrawLine(verticesX[i], verticesY[i], verticesX[j], verticesY[j],
                       outline, 4294967295U, mask);
    }

    // Draw back lines connecting bottom and top hexagons
    if (height > 0) {
        for (int i = 3; i < 6; i++) {
            game->DrawLine(verticesX[i], verticesY[i], verticesX[i],
                           verticesY[i] - height, outline, 4294967295U, mask);
        }
    }

    // Colour side faces
    if (side != olc::NONE && height > 0) {
        game->FillTriangle({ (int)verticesX[0], (int)(verticesY[0] - height) },
                           { (int)verticesX[0], (int)(verticesY[0] - 1) },
                           { (int)verticesX[1], (int)(verticesY[1] - 1) }, side,
                           mask);
        game->FillTriangle({ (int)verticesX[1], (int)(verticesY[1] - height) },
                           { (int)verticesX[0], (int)(verticesY[0] - height) },
                           { (int)verticesX[1], (int)(verticesY[1] - 1) }, side,
                           mask);
        game->FillTriangle({ (int)verticesX[1], (int)(verticesY[1] - height) },
                           { (int)verticesX[1], (int)(verticesY[1] - 1) },
                           { (int)verticesX[2], (int)(verticesY[2] - 1) }, side,
                           mask);
        game->FillTriangle({ (int)verticesX[2], (int)(verticesY[2] - height) },
                           { (int)verticesX[1], (int)(verticesY[1] - height) },
                           { (int)verticesX[2], (int)(verticesY[2] - 1) }, side,
                           mask);
        game->FillTriangle({ (int)verticesX[0], (int)(verticesY[0] - height) },
                           { (int)verticesX[0], (int)(verticesY[0] - 1) },
                           { (int)verticesX[5], (int)(verticesY[5] - 1) }, side,
                           mask);
        game->FillTriangle({ (int)verticesX[5], (int)(verticesY[5] - height) },
                           { (int)verticesX[0], (int)(verticesY[0] - height) },
                           { (int)verticesX[5], (int)(verticesY[5] - 1) }, side,
                           mask);
    }

    // Draw front lines connecting bottom and top hexagons
    for (int i = 0; i < 2; i++) {
        int j = (i + 1) % 6; // Index of the next vertex with wrapping
        game->DrawLine(verticesX[i], verticesY[i], verticesX[j], verticesY[j],
                       outline, 4294967295U, mask);
    }
    game->DrawLine(verticesX[0], verticesY[0], verticesX[5], verticesY[5],
                   outline, 4294967295U, mask);

    // Draw front lines connecting bottom and top hexagons
    if (height > 0) {
        for (int i = 0; i < 3; i++) {
            game->DrawLine(verticesX[i], verticesY[i], verticesX[i],
                           verticesY[i] - height, outline, 4294967295U, mask);
        }
    }
    game->DrawLine(verticesX[5], verticesY[5], verticesX[5],
                   verticesY[5] - height, outline, 4294967295U, mask);

    // Colour top face
    if (fill != olc::NONE) {
        game->FillTriangle(
            { (int)verticesX[0], (int)(verticesY[0] - height - 1) },
            { (int)verticesX[1], (int)(verticesY[1] - height - 1) },
            { (int)verticesX[2], (int)(verticesY[2] - height) }, fill, mask);
        game->FillTriangle(
            { (int)verticesX[0], (int)(verticesY[0] - height - 1) },
            { (int)verticesX[2], (int)(verticesY[2] - height) },
            { (int)verticesX[3], (int)(verticesY[3] - height) }, fill, mask);
        game->FillTriangle(
            { (int)verticesX[0], (int)(verticesY[0] - height - 1) },
            { (int)verticesX[3], (int)(verticesY[3] - height) },
            { (int)verticesX[4], (int)(verticesY[4] - height) }, fill, mask);
        game->FillTriangle(
            { (int)verticesX[0], (int)(verticesY[0] - height - 1) },
            { (int)verticesX[4], (int)(verticesY[4] - height) },
            { (int)verticesX[5], (int)(verticesY[5] - height) }, fill, mask);
    }

    // Draw top outline
    if (height > 0) {
        for (int i = 0; i < 6; i++) {
            int j = (i + 1) % 6; // Index of the next vertex with wrapping
            game->DrawLine(verticesX[i], verticesY[i] - height, verticesX[j],
                           verticesY[j] - height, outline, 4294967295U, mask);
        }
    }
}

void HexGrid::Draw(std::vector<std::vector<IRender *>> &renderQueue) {
    for (int i = 0; i < height; i++) {
        int r = i;
        for (int j = 0; j < width; j++) {
            int q = j - i / 2;
            olc::Pixel col = olc::DARK_GREEN;
            olc::Pixel side = olc::Pixel(139, 69, 19);
            // if (path.find(Hex(q, r)) != path.end()) {
            //     col = olc::YELLOW;
            // }
            // if (start != nullptr && start->q == q && start->r == r) {
            //     col = olc::GREEN;
            // }
            // if (end != nullptr && end->q == q && end->r == r) {
            //     col = olc::RED;
            // }
            renderQueue[r].push_back(new RenderHex(q, r, _size, olc::WHITE, col,
                                                   _heights.at(q, r), this,
                                                   nullptr, side));
        }
    }
}

void HexGrid::DrawUnits(std::vector<std::vector<IRender *>> &renderQueue,
                        int *mask) {
    for (int i = 0; i < height; i++) {
        int r = i;
        for (int j = 0; j < width; j++) {
            int q = j - i / 2;
            renderQueue[r].push_back(new RenderUnit(q, r, this, game, mask));
        }
    }
}

void HexGrid::A_Star(std::vector<Hex> &path, Hex *start, Hex *end) {
    std::priority_queue<dNode> pq;
    pq.push({ *start, 0 });

    std::unordered_map<Hex, Hex *> came_from;
    std::unordered_map<Hex, int> cost_so_far;

    came_from[*start] = nullptr;
    cost_so_far[*start] = 0;

    while (!pq.empty()) {
        dNode current = pq.top();
        pq.pop();

        if (current.u == *end) {
            break;
        }

        for (int i = 0; i < 6; i++) {
            Hex neighbor = current.u.neighbor(i);
            if (_heights.at(neighbor.q, neighbor.r) != -1 &&
                _units.at(neighbor.q, neighbor.r) == nullptr) {
                int new_cost = cost_so_far[current.u] +
                               _weights.at(neighbor.q, neighbor.r);
                if (cost_so_far.find(neighbor) == cost_so_far.end() ||
                    new_cost < cost_so_far[neighbor]) {
                    cost_so_far[neighbor] = new_cost;
                    int priority = new_cost + hex_distance(*end, neighbor);
                    pq.push({ neighbor, priority });
                    delete (came_from[neighbor]);
                    came_from[neighbor] = new Hex(current.u.q, current.u.r);
                }
            }
        }
    }

    Hex *cur = end;
    while (cur != nullptr) {
        path.push_back(*cur);
        cur = came_from[*cur];
    }
    if (path.size() > 1) {
        path.pop_back();
    }
    std::reverse(path.begin(), path.end());
}
