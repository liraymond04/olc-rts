#include "Game/RTS.h"

int main() {
    Holo::RTS game;

    if (game.Construct(384, 288, 3, 3)) {
        game.Start();
    } else {
        std::wcout << L"Could not construct console" << std::endl;
    }

    return 0;
}