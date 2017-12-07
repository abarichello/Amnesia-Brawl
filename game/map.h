#include "stdafx.h"
#include "obstacle.h"
#include "spring.h"

class Map {
public:
    Map();
    Map(std::size_t level_number, b2World& world);
    void Draw(sf::RenderWindow& window);

    void CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, sf::Texture& texture);
    void CreateAngledWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction, sf::Texture& texture);
    void CreateSpring(int posX, int posY, size_t angle);
    void ClearMap();
    void DrawSprings(sf::RenderWindow& window);

    float win_screen_countdown = 6.f;
    std::vector<Obstacle> obstacle_array;
    std::vector<Spring> spring_array;
    
private:
    void GenerateBorders(b2World& world, sf::Texture& texture);
    void LoadLevel1(b2World& world);
    void LoadLevel2(b2World& world);
    void LoadLevel3(b2World& world);

    sf::Texture neon_texture;
    sf::Texture fog_texture;
    sf::Texture spring_texture;
};
