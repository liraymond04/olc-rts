#include "IAction.h"
#include "Hex.h"
#include "Unit.h"

MoveAction::MoveAction(float targetTime, HexGrid *hexGrid, Unit *unit,
                       Hex newPos)
    : IAction(targetTime) {
    this->hexGrid = hexGrid;
    this->unit = unit;
    unit->moving = true;
    unit->path.clear();
    unit->i = 0;
    hexGrid->A_Star(unit->path, &unit->pos, &newPos);
}

void MoveAction::Tick() {
    Hex cur = unit->path[unit->i];
    if (hexGrid->_units.at(cur.q, cur.r) != nullptr) {
        Hex newPos = unit->path.back();
        if (newPos == Hex(cur.q, cur.r)) {
            done = true;
            return;
        }
        unit->path.clear();
        unit->i = 0;
        hexGrid->A_Star(unit->path, &unit->pos, &newPos);
        cur = unit->path[unit->i];
    }
    hexGrid->_units.at(unit->pos.q, unit->pos.r) = nullptr;
    hexGrid->_units.at(cur.q, cur.r) = unit;
    unit->pos = cur;
    unit->i++;
    if (unit->i >= unit->path.size()) {
        done = true;
    }
}

void MoveAction::Tock() { unit->moving = false; }