#include "stdafx.h"
#include "obstacle.h"

class Map {
public:
    Map();
    Map(std::size_t level_number, b2World& world, std::vector<Obstacle>& obstacle_array);
    
    void CreateWall(b2World& world, std::vector<Obstacle>& obstacle_array, int posX, int posY, int sizeX, int sizeY, bool is_ground);
    void CreateAngledWall(b2World& world, std::vector<Obstacle>& obstacle_array, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction);
    void ClearMap(std::vector<Obstacle>& obstacle_array);
    
private:
    void GenerateBorders(b2World& world, std::vector<Obstacle>& obstacle_array);
    void LoadLevel1(b2World& world, std::vector<Obstacle>& obstacle_array);
    void LoadLevel2(b2World& world, std::vector<Obstacle>& obstacle_array);
    void LoadLevel3(b2World& world, std::vector<Obstacle>& obstacle_array);

    // sf::Texture leaf_texture;
};
