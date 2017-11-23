#pragma once

#include "stdafx.h"
#include "player.h"
#include "obstacle.h"

class Game {
public:
    Game();

    void Start();
    void LoadSprites();
    void GameLoop(sf::RenderWindow& window, sf::Time elapsed_time);

    sf::RenderWindow window;
    sf::Clock global_clock;

    Player* player1;

    std::vector<Player*> player_array;
    std::vector<Obstacle> obstacle_array;
};
