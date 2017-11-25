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

    void CreateGround(b2World& world);
    void CreatePlayers(b2World& world, int x, int y);

    sf::RenderWindow window;
    sf::Clock global_clock;

    b2Vec2 gravity;
    b2World world;

    Player* player1;
    Obstacle ground;

    std::vector<Player*> player_array;
    std::vector<Obstacle> obstacle_array;
};
