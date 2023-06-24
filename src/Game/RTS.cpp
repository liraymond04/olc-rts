#include "RTS.h"
#include "Hex.h"
#include "Unit.h"

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
    hexGrid->_units.push_back(Unit(hexGrid, Hex(0, 0), 14, "Unit 1"));
    hexGrid->units.at(0, 0) = &hexGrid->_units[0];

    actions.push_back(new Counter(1.0f, -1));

    return true;
}

bool m_bPanning = false;
olc::vf2d m_vStartPan, m_vWorldOffset;

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

void Holo::RTS::Tick() {
    std::vector<IAction *>::iterator iter = std::begin(actions);
    while (iter != std::end(actions)) {
        (*iter)->Update(1.0f);
        if ((*iter)->done) {
            delete (*iter);
            iter = actions.erase(iter);
        } else {
            iter++;
        }
    }
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

    double height = 10;
    if (q < 5 && q >= 0 && r < 5 && r >= 0) {
        height = hexGrid->_heights.at(q, r);
    }
    hexGrid->DrawHex(q, r, hexGrid->_size, olc::RED, olc::NONE, height);
    if (GetKey(olc::Z).bPressed) {
        hexGrid->_heights.at(q, r) += 5;
        hexGrid->_weights.at(q, r) += 1;
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
    }
    if (GetKey(olc::C).bPressed) {
        hexGrid->_heights.at(q, r) -= 5;
        hexGrid->_weights.at(q, r) -= 1;
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
    }
    if (GetKey(olc::Q).bPressed) {
        if (hexGrid->_weights.at(q, r) != -1) {
            delete (hexGrid->start);
            hexGrid->start = new Hex(q, r);
        } else {
            delete (hexGrid->start);
            hexGrid->start = nullptr;
            hexGrid->path.clear();
        }
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
    }
    if (GetKey(olc::E).bPressed) {
        if (hexGrid->_weights.at(q, r) != -1) {
            delete (hexGrid->end);
            hexGrid->end = new Hex(q, r);
        } else {
            delete (hexGrid->end);
            hexGrid->end = nullptr;
            hexGrid->path.clear();
        }
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
    }

    if (GetMouse(0).bPressed) {
        if (hexGrid->_weights.at(q, r) != -1 &&
            (selected == nullptr ||
             selected != nullptr && Hex(q, r) != *selected)) {
            delete (selected);
            selected = new Hex(q, r);
        } else {
            delete (selected);
            selected = nullptr;
        }
    }
    if (selected != nullptr) {
        height = 10;
        if (selected->q < 5 && selected->q >= 0 && selected->r < 5 &&
            selected->r >= 0) {
            height = hexGrid->_heights.at(selected->q, selected->r);
        }
        hexGrid->DrawHex(selected->q, selected->r, hexGrid->_size, olc::YELLOW,
                         olc::NONE, height);

        Unit *selectedUnit = hexGrid->units.at(selected->q, selected->r);
        if (selectedUnit != nullptr) {
            DrawString({ 10, ScreenHeight() - 15 }, selectedUnit->name);
        }

        if (selectedUnit != nullptr && GetMouse(1).bPressed &&
            hexGrid->_weights.at(q, r) != -1) {
            // selectedUnit->moving = true;
            hexGrid->units.at(selected->q, selected->r) = nullptr;
            hexGrid->units.at(q, r) = selectedUnit;
            selectedUnit->pos = Hex(q, r);
            delete (selected);
            selected = new Hex(q, r);
        }
    }

    if (GetKey(olc::G).bPressed) {
        actions[0]->done = true;
    }

    for (Unit &unit : hexGrid->_units) {
        unit.Draw(this);
    }

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
