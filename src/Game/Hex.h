#ifndef HEX_H
#define HEX_H

#include <iostream>
#include <string>
#include <algorithm>

#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "RTS.h"

struct Hex {
    static vector<Hex> directions, diagonals;

    int q, r, s;
    Hex(int q_, int r_) : q(q_), r(r_), s(-q_ - r_) {}

    friend bool operator==(const Hex &lhs, const Hex &rhs) {
        return lhs.q == rhs.q && lhs.r == rhs.r;
    }
    friend bool operator!=(const Hex &lhs, const Hex &rhs) {
        return !(lhs == rhs);
    }

    Hex operator+(Hex const &a) { return Hex(q + a.q, r + a.r); }
    Hex operator-(Hex const &a) { return Hex(q - a.q, r - a.r); }
    Hex operator*(Hex const &a) { return Hex(q * a.q, r * a.r); }

    Hex scale(int k) { return Hex(q * k, r * k); }
    Hex rotate_l() { return Hex(-s, -q); }
    Hex rotate_r() { return Hex(-r, -s); }

    Hex neighbor(int direction) { return Hex(q, r) + directions[direction]; }
    Hex diagonal(int direction) { return Hex(q, r) + diagonals[direction]; }

    int length() { return int((abs(q) + abs(r) + abs(s)) / 2); }
};

namespace std {
template <> struct hash<Hex> {
    size_t operator()(const Hex &h) const {
        hash<int> int_hash;
        size_t hq = int_hash(h.q);
        size_t hr = int_hash(h.r);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
};
} // namespace std

struct FractionalHex {
    const double q, r, s;
    FractionalHex(double q_, double r_) : q(q_), r(r_), s(-q_ - r_) {}
};

template <class T> class HexMap {
    std::vector<std::vector<T>> map;

    int left_, top_;
    T null_val;

  public:
    HexMap(int left, int top, int right, int bottom, T init_val, T null_val)
        : left_(left), top_(top) {
        this->null_val = null_val;
        int height = bottom - top + 1;
        map.resize(height);
        for (int r = 0; r < height; r++) {
            int width = right - left + 1;
            std::vector<T> row(width, init_val);
            map[r] = row;
        }
    }

    inline T &at(int q, int r) {
        int j = r - top_;
        int i = q - left_ + (r >> 1);
        if (j < 0 || j >= map.size() || i < 0 || i >= map[0].size()) {
            return null_val;
        }
        return map[j][i];
    }
};

static int hex_distance(Hex a, Hex b) { return (a - b).length(); }

struct dNode {
    Hex u;
    int dist;
    friend bool operator<(const dNode &l, const dNode &r) {
        return l.dist > r.dist;
    }
};

class Unit;

class HexGrid {
  private:
    Holo::RTS *game;

  public:
    int width, height;
    double _size;

    HexMap<int> _heights{ 0, 0, 4, 4, 10, -1 };
    HexMap<int> _weights{ 0, 0, 4, 4, 1, -1 };
    HexMap<Unit *> units{ 0, 0, 4, 4, nullptr, nullptr };

    std::vector<Unit> _units;

    double scale = 1.0;
    double rotation_deg = 10.0;
    double shear_x = 0.0;
    double shear_y = 0.0;
    double squeeze_x = 1.0;
    double squeeze_y = 0.5;
    double translate_x = 0.0f;
    double translate_y = 0.0f;

    Hex *start = nullptr, *end = nullptr;
    std::unordered_set<Hex> path;

    HexGrid(Holo::RTS *game, int width, int height, double _size) {
        this->game = game;
        this->width = width;
        this->height = height;
        this->_size = _size;
    }

    void DrawHex(int q, int r, double sideLength, olc::Pixel color = olc::WHITE,
                 olc::Pixel fill = olc::NONE, double height = 0);

    void Draw();

    void A_Star();

    void ConvertToIsometric(double &x, double &y);
    void ConvertToCartesian(double &x, double &y);
    void CalculateIsometricHexagonVertices(double centerX, double centerY,
                                           double sideLength,
                                           double verticesX[],
                                           double verticesY[]);
    void CalculateHexagonCenter(int q, int r, double size, double &centerX,
                                double &centerY);
    void CalculateAxialCoordinates(double centerX, double centerY, double size,
                                   double &q, double &r);
    void CalculateIsometricAxialCoordinates(double centerX, double centerY,
                                            double size, double &q, double &r);
};

#endif // !HEX_H