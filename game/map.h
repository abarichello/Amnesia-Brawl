#include "stdafx.h"
#include "obstacle.h"
#include "spring.h"

class Map {
public:
    Map();
    Map(std::size_t level_number, b2World& world, std::vector<Obstacle>& obstacle_array);
    
    void CreateWall(b2World& world, std::vector<Obstacle>& obstacle_array, int posX, int posY, int sizeX, int sizeY, bool is_ground, sf::Texture& texture);
    void CreateAngledWall(b2World& world, std::vector<Obstacle>& obstacle_array, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction, sf::Texture& texture);
    void CreateSpring(std::vector<Obstacle>& obstacle_array, int posX, int posY, size_t angle);
    void ClearMap(std::vector<Obstacle>& obstacle_array);
    void DrawSprings(sf::RenderWindow& window);

    float win_screen_countdown = 6.f;
    std::vector<Spring> spring_array;
    
private:
    void GenerateBorders(b2World& world, std::vector<Obstacle>& obstacle_array, sf::Texture& texture);
    void LoadLevel1(b2World& world, std::vector<Obstacle>& obstacle_array);
    void LoadLevel2(b2World& world, std::vector<Obstacle>& obstacle_array);
    void LoadLevel3(b2World& world, std::vector<Obstacle>& obstacle_array);

    sf::Texture neon_texture;
    sf::Texture fog_texture;
    sf::Texture spring_texture;
};
