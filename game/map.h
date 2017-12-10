#pragma once

#include "stdafx.h"
#include "obstacle.h"
#include "spring.h"

class Map {
public:
    Map();
    Map(std::size_t level_number, b2World& world);
    void Draw(sf::RenderWindow& window);
    void Draw(sf::RenderTexture& texture);

    void CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, sf::Texture& texture, sf::Color color);
    void CreateAngledWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction, sf::Texture& texture, sf::Color color);
    void CreateSpring(int posX, int posY, size_t angle);
    void ClearMap();
    void DrawSprings(sf::RenderWindow& window);

    float win_screen_countdown = 7.f;
    std::size_t level_number;

    std::vector<Obstacle> obstacle_array;
    std::vector<Spring> spring_array;
    
private:
    std::size_t pulse_variable = 0;

    void GenerateBorders(b2World& world, sf::Texture& texture);
    void LoadLevel1(b2World& world);
    void LoadLevel2(b2World& world);
    void LoadLevel3(b2World& world);
    
    sf::Image degrade;

    sf::Texture background_texture;
    sf::Texture amnesia_texture;
    sf::Texture neon_texture;
    sf::Texture fog_texture;
    sf::Texture spring_texture;
    
    sf::Sprite background_sprite;
    sf::Sprite amnesia_logo;
};
