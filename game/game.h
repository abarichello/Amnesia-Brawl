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

    void CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY);
    void CreatePlayers(b2World& world, Player* player, int x, int y);

    sf::RenderWindow window;
    sf::Clock global_clock;

    b2Vec2 gravity;
    b2World world;

    Player* player1;
    Player* player2;
    Obstacle wall;

    std::vector<Player*> player_array;
    std::vector<Obstacle> obstacle_array;
};
