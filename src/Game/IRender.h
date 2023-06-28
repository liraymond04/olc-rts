#ifndef IRENDER_H
#define IRENDER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "RTS.h"
#include "Hex.h"
#include "Unit.h"

class IRender {
  public:
    IRender() {}

    virtual void Draw() = 0;
};

class RenderLine : public IRender {
  private:
    olc::vi2d pos1;
    olc::vi2d pos2;
    olc::Pixel p;
    Holo::RTS *game;

  public:
    RenderLine(const olc::vi2d &pos1, const olc::vi2d &pos2,
               olc::Pixel p = olc::WHITE, Holo::RTS *game = nullptr) {
        this->pos1 = pos1;
        this->pos2 = pos2;
        this->p = p;
        this->game = game;
    }

    void Draw() override { game->DrawLine(pos1, pos2, p); }
};

class RenderHex : public IRender {
  private:
    int q;
    int r;
    double sideLength;
    olc::Pixel color;
    olc::Pixel fill;
    double height;
    HexGrid *hexGrid;

  public:
    RenderHex(int q, int r, double sideLength, olc::Pixel color = olc::WHITE,
              olc::Pixel fill = olc::NONE, double height = 0.0f,
              HexGrid *hexGrid = nullptr) {
        this->q = q;
        this->r = r;
        this->sideLength = sideLength;
        this->color = color;
        this->fill = fill;
        this->height = height;
        this->hexGrid = hexGrid;
    }

    void Draw() override {
        hexGrid->DrawHex(q, r, sideLength, color, fill, height);
    }
};

class RenderUnit : public IRender {
  private:
    int q;
    int r;
    HexGrid *hexGrid;
    Holo::RTS *game;

  public:
    RenderUnit(int q, int r, HexGrid *hexGrid, Holo::RTS *game) {
        this->q = q;
        this->r = r;
        this->hexGrid = hexGrid;
        this->game = game;
    }

    void Draw() override {
        if (hexGrid->_units.at(q, r) != nullptr) {
            hexGrid->_units.at(q, r)->Draw(game);
        }
    }
};

#endif // ! IRENDER_H