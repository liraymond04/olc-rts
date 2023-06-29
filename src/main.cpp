#include "Game/RTS.h"

int main() {
    Holo::RTS game;

    if (game.Construct(512, 384, 2, 2)) {
        game.Start();
    } else {
        std::wcout << L"Could not construct console" << std::endl;
    }

    return 0;
}