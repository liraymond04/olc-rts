#include "IAction.h"
#include "Hex.h"
#include "Unit.h"

MoveAction::MoveAction(float targetTime, HexGrid *hexGrid, Unit *unit,
                       Hex startPos, Hex endPos)
    : IAction(targetTime) {
    this->hexGrid = hexGrid;
    this->unit = unit;

    if (unit->path.empty()) {
        hexGrid->A_Star(unit->path, &startPos, &endPos);
    } else {
        std::vector<Hex> p;
        hexGrid->A_Star(p, &startPos, &endPos);
        unit->queued.push_back(p);
    }
}

void MoveAction::Start() {
    unit->moving = true;
    if (!unit->queued.empty()) {
        unit->path = unit->queued.front();
        unit->queued.erase(unit->queued.begin());
    }
    unit->i = 0;
    started = true;
}

void MoveAction::Tick() {
    Hex cur = unit->path[unit->i];
    if (hexGrid->_units.at(cur.q, cur.r) != nullptr &&
        hexGrid->_units.at(cur.q, cur.r) != unit) {
        Hex newPos = unit->path.back();
        if (newPos == Hex(cur.q, cur.r)) {
            done = true;
            unit->path.clear();
            unit->queued.clear();
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

void MoveAction::Tock() {
    unit->moving = false;
    unit->path.clear();
}