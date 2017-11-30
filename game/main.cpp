#include "game.h"
#include <iostream>

int main() {
    srand(time(0));
    Game* game = new Game();
    game->Start();
}
