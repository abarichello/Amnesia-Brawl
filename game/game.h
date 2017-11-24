#pragma once

#include "stdafx.h"
#include "player.h"
#include "obstacle.h"

#include <utility>

class Game {
public:
    Game();

    void Start();
    void LoadResources();
    void GameLoop(sf::RenderWindow& window, sf::Time elapsed_time);

    void CreateGround(b2World& world);
    void CreatePlayers(b2World& world, int x, int y);

    sf::RenderWindow window;
    sf::Clock global_clock;

    b2World world;
    b2Vec2 gravity;

    Player* player1;
    Obstacle ground;

    std::vector<Player*> player_array;
    std::vector<Obstacle> obstacle_array;
};
