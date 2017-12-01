#pragma once

#include "stdafx.h"
#include "player.h"
#include "obstacle.h"
#include "game_object_manager.h"
#include "hud.h"

#include <utility>

class Game {
public:
    Game();

    void Start();
    void LoadResources();
    void LoadPlayers(std::size_t number_of_players);
    void SpawnPlayer(std::size_t number, Player* player, sf::Color color, sf::Keyboard::Key jump, sf::Keyboard::Key left, sf::Keyboard::Key right);

    void CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground);
    void CreateBall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground);
    void CreatePlayer(b2World& world, Player* player, int x, int y);

    sf::RenderWindow window;
    sf::Clock global_clock;
    
    b2Vec2 gravity;
    b2World world;
    HUD hud;
    
    Player* player1;
    Player* player2;
    Player* player3;
    Player* player4;
    Obstacle wall;
    
    sf::Color amnesia_blue;
    sf::Color amnesia_red;
    sf::Color amnesia_dark_blue;
    sf::Color amnesia_dark_red;

    std::vector<Player*> player_array;
    std::vector<Obstacle> obstacle_array;

    std::vector<b2Vec2> spawn_locations;

    static GameObjectManager _game_object_manager;
};
