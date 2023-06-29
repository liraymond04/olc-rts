#ifndef RTS_H
#define RTS_H

#include <iostream>
#include <string>
#include <algorithm>

#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_QuickGUI.h"

class HexGrid;

// server client model
// server thread, client thread
// network

// tile movement lerp based on unit speed
// while center in tile, is unit pos

// cull hexes outside of screen borders

namespace Holo {
class RTS : public olc::PixelGameEngine {
  public:
    RTS() { sAppName = "Holo::RTS"; }

  private:
    bool showSliders = false;

    olc::QuickGUI::Manager guiManager;

    olc::QuickGUI::Slider *guiSlider1 = nullptr;
    olc::QuickGUI::Slider *guiSlider2 = nullptr;
    olc::QuickGUI::Slider *guiSlider3 = nullptr;
    olc::QuickGUI::Slider *guiSlider4 = nullptr;
    olc::QuickGUI::Slider *guiSlider5 = nullptr;
    olc::QuickGUI::Slider *guiSlider6 = nullptr;

    HexGrid *hexGrid;

    float fTargetFrameTime = 1.0f / 60.0f; // Virtual FPS of 60fps
    float fAccumulatedTime = 0.0f;

  public:
    void Tick();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};
} // namespace Holo

#endif // !RTS_H