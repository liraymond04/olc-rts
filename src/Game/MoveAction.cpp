#include "IAction.h"
#include "Hex.h"
#include "Unit.h"

MoveAction::MoveAction(float targetTime, HexGrid *hexGrid, Unit *unit,
                       Hex newPos)
    : IAction(targetTime) {
    this->hexGrid = hexGrid;
    this->unit = unit;
    unit->moving = true;
    hexGrid->A_Star(path, &unit->pos, &newPos);
}

void MoveAction::Tick() {
    if (i < path.size()) {
        Hex cur = path[i];
        hexGrid->units.at(unit->pos.q, unit->pos.r) = nullptr;
        hexGrid->units.at(cur.q, cur.r) = unit;
        unit->pos = cur;
        i++;
    } else {
        done = true;
    }
}

void MoveAction::Tock() { unit->moving = false; }