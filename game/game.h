#include "stdafx.h"

class Game {
public:
    Game();

    void Start();
    void GameLoop(sf::RenderWindow& window, sf::Time elapsed_time);

    sf::RenderWindow window;
    sf::Clock global_clock;
};
