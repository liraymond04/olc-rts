#include "RTS.h"
#include "Hex.h"

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

bool Holo::RTS::OnUserUpdate(float fElapsedTime) {
    guiManager.Update(this);

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
    if (GetMouse(0).bPressed) {
        hexGrid->_heights.at(q, r) += 5;
        hexGrid->_weights.at(q, r) += 1;
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
    }
    if (GetMouse(1).bPressed) {
        hexGrid->_heights.at(q, r) -= 5;
        hexGrid->_weights.at(q, r) -= 1;
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
    }
    if (GetKey(olc::Q).bPressed) {
        hexGrid->start = new Hex(q, r);
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
    }
    if (GetKey(olc::E).bPressed) {
        hexGrid->end = new Hex(q, r);
        if (hexGrid->start != nullptr && hexGrid->end != nullptr) {
            hexGrid->A_Star();
        }
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

    // if (GetKey(olc::Q).bHeld) {
    //     hexGrid->_size += 25.0f * fElapsedTime;
    // }
    // if (GetKey(olc::E).bHeld) {
    //     hexGrid->_size -= 25.0f * fElapsedTime;
    // }

    return true;
}
