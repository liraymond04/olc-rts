#ifndef UNIT_H
#define UNIT_H

#include "RTS.h"
#include "Hex.h"

class IAction;

class Unit {
  private:
    /* data */
    HexGrid *hexGrid;

  public:
    Hex pos;
    double size;

    bool moving = false;
    std::vector<Hex> path;
    std::vector<std::vector<Hex>> queued;
    int i = 0;

    std::string name;
    olc::Pixel color;

    std::vector<IAction *> actions;

    Unit(HexGrid *hexGrid, Hex _pos, double size, std::string name,
         olc::Pixel color)
        : pos(_pos) {
        this->hexGrid = hexGrid;
        this->size = size;
        this->name = name;
        this->color = color;
    }

    void Draw(Holo::RTS *game, int *mask = nullptr) {
        double centerX, centerY;
        hexGrid->CalculateHexagonCenter(pos.q, pos.r, hexGrid->_size, centerX,
                                        centerY);
        hexGrid->ConvertToIsometric(centerX, centerY);
        game->FillCircle(
            { (int)centerX,
              (int)(centerY - hexGrid->_heights.at(pos.q, pos.r) - size / 2) },
            size, color);
        if (mask != nullptr) {
            game->MaskCircleFill(
                mask,
                { (int)centerX,
                  (int)(centerY - hexGrid->_heights.at(pos.q, pos.r) -
                        size / 2) },
                size, color);
        }
    }
};

#endif // !UNIT_H
