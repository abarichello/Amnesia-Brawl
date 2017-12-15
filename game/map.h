#pragma once

#include "stdafx.h"
#include "obstacle.h"
#include "spring.h"
#include "teleport_pad.h"

class Map {
public:
    Map();
    Map(std::size_t level_number, b2World& world);
    ~Map();
    void Update();
    void Draw(sf::RenderWindow& window);

    void CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction, sf::Texture& texture, sf::Color color, b2BodyType type);
    void CreateSpring(int posX, int posY, size_t angle, sf::Color color);
    void CreateTeleport(int posX, int posY, b2Vec2 destination, sf::Color color);

    float win_screen_countdown = 7.f;
    std::size_t level_number;

    std::vector<Obstacle> obstacle_array;
    std::vector<Spring> spring_array;
    std::vector<TeleportPad> tpad_array;

private:
    std::size_t pulse_variable = 0;

    void GenerateBorders(b2World& world, sf::Texture& texture);
    void LoadLevel1(b2World& world);
    void LoadLevel2(b2World& world);
    void LoadLevel3(b2World& world);

    sf::Image degrade;

    sf::Texture background_texture;
    sf::Texture amnesia_texture;
    sf::Texture package_texture;
    sf::Texture neon_texture;
    sf::Texture desert_block_texture;
    sf::Texture fog_texture;
    sf::Texture spring_texture;

    sf::Sprite background_sprite;
    sf::Sprite amnesia_logo;
};
