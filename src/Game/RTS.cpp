#include "RTS.h"
#include "Hex.h"
#include "Unit.h"
#include "IAction.h"

bool Holo::RTS::OnUserCreate() {
    guiSlider1 = new olc::QuickGUI::Slider(guiManager, { 30.0f, 10.0f },
                                           { 246.0f, 10.0f }, 1, 2, 1);
    guiSlider2 = new olc::QuickGUI::Slider(guiManager, { 30.0f, 30.0f },
                                           { 246.0f, 30.0f }, -360, 360, 10);
    guiSlider3 = new olc::QuickGUI::Slider(guiManager, { 30.0f, 50.0f },
                                           { 246.0f, 50.0f }, 0, 1, 0);
    guiSlider4 = new olc::QuickGUI::Slider(guiManager, { 30.0f, 70.0f },
                                           { 246.0f, 70.0f }, 0, 1, 0);
    guiSlider5 = new olc::QuickGUI::Slider(guiManager, { 30.0f, 90.0f },
                                           { 246.0f, 90.0f }, 0, 1, 1);
    guiSlider6 = new olc::QuickGUI::Slider(guiManager, { 30.0f, 110.0f },
                                           { 246.0f, 110.0f }, 0, 1, 0.5f);

    hexGrid = new HexGrid(this, 5, 5, 30);
    hexGrid->units.push_back(
        new Unit(hexGrid, Hex(0, 0), 14, "Unit 1", olc::BLUE));
    hexGrid->_units.at(0, 0) = hexGrid->units[0];
    hexGrid->units.push_back(
        new Unit(hexGrid, Hex(0, 1), 14, "Unit 2", olc::DARK_RED));
    hexGrid->_units.at(0, 1) = hexGrid->units[1];

    // actions.push_back(new Counter(1.0f, -1));

    return true;
}

bool m_bPanning = false;
olc::vf2d m_vStartPan, m_vWorldOffset = { -150, -140 };

void StartPan(const olc::vi2d &vPos) {
    m_bPanning = true;
    m_vStartPan = olc::vf2d(vPos);
}

void UpdatePan(const olc::vi2d &vPos) {
    if (m_bPanning) {
        m_vWorldOffset -= (olc::vf2d(vPos) - m_vStartPan);
        m_vStartPan = olc::vf2d(vPos);
    }
}

void EndPan(const olc::vi2d &vPos) {
    UpdatePan(vPos);
    m_bPanning = false;
}

Hex *selected = nullptr;
Unit *following = nullptr;

void Holo::RTS::Tick() {
    std::vector<IAction *> add;

    std::vector<IAction *>::iterator iter = std::begin(hexGrid->actions);
    while (iter != std::end(hexGrid->actions)) {
        if (!(*iter)->started) {
            (*iter)->Start();
        }
        (*iter)->Update(1.0f);
        if ((*iter)->done) {
            MoveAction *action = (MoveAction *)(*iter);
            action->unit->actions.erase(action->unit->actions.begin());
            if (!action->unit->actions.empty()) {
                add.push_back(action->unit->actions.front());
            }
            delete (*iter);
            iter = hexGrid->actions.erase(iter);
        } else {
            iter++;
        }
    }

    hexGrid->actions.insert(hexGrid->actions.end(), add.begin(), add.end());
}

bool Holo::RTS::OnUserUpdate(float fElapsedTime) {
    guiManager.Update(this);

    bool tick = true;
    fAccumulatedTime += fElapsedTime;
    float frameTime = fAccumulatedTime;
    if (fAccumulatedTime > fTargetFrameTime) {
        fAccumulatedTime -= fTargetFrameTime;
    } else {
        tick = false;
    }
    if (tick) {
        // std::cout << 1 / frameTime << "\n";
        Tick();
    }

    Clear(olc::BLACK);

    hexGrid->translate_x = m_vWorldOffset.x;
    hexGrid->translate_y = m_vWorldOffset.y;
    hexGrid->Draw();

    double q = 0, r = 0;
    hexGrid->CalculateIsometricAxialCoordinates(GetMouseX(), GetMouseY(),
                                                hexGrid->_size, q, r);

    for (Unit *unit : hexGrid->units) {
        if (unit->moving) {
            // hexGrid->DrawHex(unit->pos.q, unit->pos.r, hexGrid->_size,
            //                  olc::YELLOW, olc::NONE,
            //                  hexGrid->_heights.at(unit->pos.q, unit->pos.r));
            double prevX, prevY;
            int prevPosX = unit->pos.q, prevPosY = unit->pos.r;
            hexGrid->CalculateHexagonCenter(prevPosX, prevPosY, hexGrid->_size,
                                            prevX, prevY);
            hexGrid->ConvertToIsometric(prevX, prevY);
            for (int i = unit->i; i < unit->path.size(); i++) {
                Hex hex = unit->path[i];
                double centerX, centerY;
                hexGrid->CalculateHexagonCenter(hex.q, hex.r, hexGrid->_size,
                                                centerX, centerY);
                hexGrid->ConvertToIsometric(centerX, centerY);
                DrawLine({ (int)centerX,
                           (int)centerY - hexGrid->_heights.at(hex.q, hex.r) },
                         { (int)prevX, (int)prevY - hexGrid->_heights.at(
                                                        prevPosX, prevPosY) },
                         olc::YELLOW);
                prevX = centerX;
                prevY = centerY;
                prevPosX = hex.q;
                prevPosY = hex.r;
                // hexGrid->DrawHex(hex.q, hex.r, hexGrid->_size, olc::YELLOW,
                //                  olc::NONE, hexGrid->_heights.at(hex.q,
                //                  hex.r));
            }
            for (std::vector<Hex> &p : unit->queued) {
                for (Hex hex : p) {
                    double centerX, centerY;
                    hexGrid->CalculateHexagonCenter(
                        hex.q, hex.r, hexGrid->_size, centerX, centerY);
                    hexGrid->ConvertToIsometric(centerX, centerY);
                    DrawLine(
                        { (int)centerX,
                          (int)centerY - hexGrid->_heights.at(hex.q, hex.r) },
                        { (int)prevX,
                          (int)prevY - hexGrid->_heights.at(hex.q, hex.r) },
                        olc::CYAN);
                    prevX = centerX;
                    prevY = centerY;
                }
            }
        }
    }

    double height = 10;
    if (q < 5 && q >= 0 && r < 5 && r >= 0) {
        height = hexGrid->_heights.at(q, r);
    }
    hexGrid->DrawHex(q, r, hexGrid->_size, olc::RED, olc::NONE, height);
    if (GetKey(olc::Z).bPressed) {
        hexGrid->_heights.at(q, r) += 5;
        hexGrid->_weights.at(q, r) += 1;
    }
    if (GetKey(olc::C).bPressed) {
        hexGrid->_heights.at(q, r) -= 5;
        hexGrid->_weights.at(q, r) -= 1;
    }

    if (following != nullptr) {
        delete (selected);
        selected = new Hex(following->pos.q, following->pos.r);
    }
    if (GetMouse(0).bPressed) {
        if (hexGrid->_weights.at(q, r) != -1 &&
            (selected == nullptr ||
             selected != nullptr && Hex(q, r) != *selected)) {
            delete (selected);
            selected = new Hex(q, r);
            following = nullptr;
        } else {
            delete (selected);
            selected = nullptr;
            following = nullptr;
        }
    }
    if (selected != nullptr) {
        height = 10;
        if (selected->q < 5 && selected->q >= 0 && selected->r < 5 &&
            selected->r >= 0) {
            height = hexGrid->_heights.at(selected->q, selected->r);
        }
        hexGrid->DrawHex(selected->q, selected->r, hexGrid->_size, olc::CYAN,
                         olc::NONE, height);

        Unit *selectedUnit = hexGrid->_units.at(selected->q, selected->r);
        if (selectedUnit != nullptr) {
            following = selectedUnit;
            DrawString({ 10, ScreenHeight() - 15 }, selectedUnit->name);
            std::vector<Hex> path;
            Hex end = Hex(q, r);
            if (selectedUnit->actions.empty()) {
                hexGrid->A_Star(path, &selectedUnit->pos, &end);
            } else if (!selectedUnit->path.empty()) {
                if (selectedUnit->queued.empty()) {
                    hexGrid->A_Star(path, &selectedUnit->path.back(), &end);
                } else {
                    hexGrid->A_Star(path, &selectedUnit->queued.back().back(),
                                    &end);
                }
            }
            // for (Hex hex : path) {
            //     hexGrid->DrawHex(hex.q, hex.r, hexGrid->_size, olc::YELLOW,
            //                      olc::NONE, hexGrid->_heights.at(hex.q,
            //                      hex.r));
            // }
            double prevX, prevY;
            int _q = selectedUnit->pos.q, _r = selectedUnit->pos.r;
            if (!selectedUnit->path.empty()) {
                if (selectedUnit->queued.empty()) {
                    _q = selectedUnit->path.back().q;
                    _r = selectedUnit->path.back().r;
                } else {
                    _q = selectedUnit->queued.back().back().q;
                    _r = selectedUnit->queued.back().back().r;
                }
            }

            hexGrid->CalculateHexagonCenter(_q, _r, hexGrid->_size, prevX,
                                            prevY);
            hexGrid->ConvertToIsometric(prevX, prevY);
            if (hexGrid->_heights.at(q, r) != -1) {
                for (Hex hex : path) {
                    double centerX, centerY;
                    hexGrid->CalculateHexagonCenter(
                        hex.q, hex.r, hexGrid->_size, centerX, centerY);
                    hexGrid->ConvertToIsometric(centerX, centerY);
                    DrawLine(
                        { (int)centerX,
                          (int)centerY - hexGrid->_heights.at(hex.q, hex.r) },
                        { (int)prevX,
                          (int)prevY - hexGrid->_heights.at(_q, _r) },
                        olc::CYAN);
                    prevX = centerX;
                    prevY = centerY;
                    _q = hex.q;
                    _r = hex.r;
                }
            }
            height = 10;
            if (q < 5 && q >= 0 && r < 5 && r >= 0) {
                height = hexGrid->_heights.at(q, r);
            }
            hexGrid->DrawHex(q, r, hexGrid->_size, olc::CYAN, olc::NONE,
                             height);
        }

        if (selectedUnit != nullptr && GetMouse(1).bPressed &&
            hexGrid->_weights.at(q, r) != -1 &&
            hexGrid->_units.at(q, r) == nullptr) {
            // selectedUnit->moving = true;
            // hexGrid->units.at(selected->q, selected->r) = nullptr;
            // hexGrid->units.at(q, r) = selectedUnit;
            // selectedUnit->pos = Hex(q, r);
            if (selectedUnit->actions.empty()) {
                hexGrid->actions.push_back(new MoveAction(
                    1.0f, hexGrid, selectedUnit, selectedUnit->pos, Hex(q, r)));
                selectedUnit->actions.push_back(hexGrid->actions.back());
            } else {
                Hex start = selectedUnit->path.back();
                if (!selectedUnit->queued.empty()) {
                    start = selectedUnit->queued.back().back();
                }
                selectedUnit->actions.push_back(new MoveAction(
                    1.0f, hexGrid, selectedUnit, start, Hex(q, r)));
            }
            // delete (selected);
            // selected = new Hex(q, r);
        }
    }

    // for (Unit *unit : hexGrid->_units) {
    //     unit->Draw(this);
    // }
    hexGrid->DrawUnits();

    if (GetKey(olc::TAB).bPressed) {
        showSliders = !showSliders;
    }
    if (showSliders) {
        guiManager.Draw(this);
    }

    const auto &vMousePos = GetMousePos();

    if (GetMouse(2).bPressed)
        StartPan(vMousePos);
    if (GetMouse(2).bHeld)
        UpdatePan(vMousePos);
    if (GetMouse(2).bReleased)
        EndPan(vMousePos);

    if (GetKey(olc::A).bHeld) {
        hexGrid->_size += 25.0f * fElapsedTime;
    }
    if (GetKey(olc::D).bHeld) {
        hexGrid->_size -= 25.0f * fElapsedTime;
    }

    return true;
}
