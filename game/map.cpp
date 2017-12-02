#include "map.h"

Map::Map() = default;

Map::Map(std::size_t level_number, b2World& world, std::vector<Obstacle>& obstacle_array) {
    switch(level_number) {
        case 1:
            LoadLevel1(world, obstacle_array);
            break;
        case 2:
            LoadLevel2(world, obstacle_array);
            break;
        case 3:
            LoadLevel3(world, obstacle_array);
            break;
    }
}

void Map::LoadLevel1(b2World& world, std::vector<Obstacle>& obstacle_array) {
    //         World                                          posX                           posY         sizeX           sizeY  ground?
    // Platforms
    CreateWall(world, obstacle_array,                GAME_WIDTH/4,   GAME_HEIGHT - GAME_HEIGHT/6,  GAME_WIDTH/4, GAME_HEIGHT/40, true); // Lower left
    CreateWall(world, obstacle_array,  GAME_WIDTH - GAME_WIDTH/20,   GAME_HEIGHT - GAME_HEIGHT/3,  GAME_WIDTH/9, GAME_HEIGHT/35, true); // Lower right
    CreateWall(world, obstacle_array, GAME_WIDTH/2 - GAME_WIDTH/3, GAME_HEIGHT/2 - GAME_HEIGHT/4, GAME_WIDTH/10, GAME_HEIGHT/45, true); // Upper platform
    CreateWall(world, obstacle_array,   GAME_WIDTH - GAME_WIDTH/2,                 GAME_HEIGHT/2,  GAME_WIDTH/3, GAME_HEIGHT/40, true); // Lower right

    GenerateBorders(world, obstacle_array);
}

void Map::LoadLevel2(b2World& world, std::vector<Obstacle>& obstacle_array) {
}

void Map::LoadLevel3(b2World& world, std::vector<Obstacle>& obstacle_array) {
}

void Map::GenerateBorders(b2World& world, std::vector<Obstacle>& obstacle_array) {
    //         World                         posX              posY         sizeX           sizeY  ground?
    // Boundaries
    CreateWall(world, obstacle_array, GAME_WIDTH/2,    GAME_HEIGHT,    GAME_WIDTH, GAME_HEIGHT/40, true); // Ground
    CreateWall(world, obstacle_array,            0,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, true); // Left wall
    CreateWall(world, obstacle_array,   GAME_WIDTH,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, true); // Right wall
    CreateWall(world, obstacle_array, GAME_WIDTH/2,              0,    GAME_WIDTH, GAME_HEIGHT/40, false); // Ceiling
}

void Map::CreateWall(b2World& world, std::vector<Obstacle>& obstacle_array, int posX, int posY, int sizeX, int sizeY, bool is_ground) {
    Obstacle wall;
    wall.rect.setSize(sf::Vector2f(sizeX, sizeY));
    wall.rect.setPosition(posX, posY);
    wall.rect.setOrigin(wall.rect.getSize().x / 2, wall.rect.getSize().y / 2);
    wall.is_ground = is_ground;

    wall.bodydef.type = b2_staticBody;
    wall.bodydef.position.Set(wall.rect.getPosition().x / SCALE, wall.rect.getPosition().y / SCALE);
    wall.fixturedef.shape = &wall.shape;
    wall.fixturedef.density = 0.f;
    wall.shape.SetAsBox(wall.rect.getLocalBounds().width / 2 / SCALE, wall.rect.getLocalBounds().height / 2 / SCALE);

    wall.body = world.CreateBody(&wall.bodydef);
    wall.body->CreateFixture(&wall.fixturedef);
    obstacle_array.push_back(wall);
}
