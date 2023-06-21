#ifndef RTS_H
#define RTS_H

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

#include "olcPixelGameEngine.h"
#include "olcPGEX_QuickGUI.h"

class HexGrid;

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

  public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};
} // namespace Holo

#endif // !RTS_H