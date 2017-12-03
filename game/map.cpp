#include "map.h"

Map::Map() = default;

Map::Map(std::size_t level_number, b2World& world, std::vector<Obstacle>& obstacle_array) {
    switch(level_number) {
        case 1:
            LoadLevel1(world, obstacle_array); // PLATFORM LEVEL
            break;
        case 2:
            LoadLevel2(world, obstacle_array); // ABISS LEVEL
            break;
        case 3:
            LoadLevel3(world, obstacle_array); // TREE BRANCHES LEVEL
            break;
    }
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

void Map::CreateAngledWall(b2World& world, std::vector<Obstacle>& obstacle_array, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction) {
    Obstacle wall;
    b2BodyDef bodydef;
    wall.rect.setSize(sf::Vector2f(sizeX, sizeY));
    wall.rect.setPosition(posX, posY);
    wall.rect.setOrigin(wall.rect.getSize().x / 2, wall.rect.getSize().y / 2);
    wall.rect.setRotation(angle);
    wall.is_ground = is_ground;

    bodydef.type = b2_staticBody;
    bodydef.position.Set(wall.rect.getPosition().x / SCALE, wall.rect.getPosition().y / SCALE);
    bodydef.angle = angle * 0.01745329f; // Degree to radians
    wall.fixturedef.shape = &wall.shape;
    wall.fixturedef.friction = friction;
    wall.shape.SetAsBox(wall.rect.getLocalBounds().width / 2 / SCALE, wall.rect.getLocalBounds().height / 2 / SCALE);

    wall.body = world.CreateBody(&bodydef);
    wall.body->CreateFixture(&wall.fixturedef);
    obstacle_array.push_back(wall);
}

void Map::DrawSprings(sf::RenderWindow& window) {
    for (auto spring : spring_array) {
        window.draw(spring.rect);
    }
}

void Map::ClearMap(std::vector<Obstacle>& obstacle_array) {
    for (auto wall : obstacle_array) {
        obstacle_array.pop_back();
    }
}

void Map::GenerateBorders(b2World& world, std::vector<Obstacle>& obstacle_array) {
    //         World                         posX              posY         sizeX           sizeY  ground?
    // Boundaries
    CreateWall(world, obstacle_array, GAME_WIDTH/2,    GAME_HEIGHT,    GAME_WIDTH, GAME_HEIGHT/40, true); // Ground
    CreateWall(world, obstacle_array,            0,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, true); // Left wall
    CreateWall(world, obstacle_array,   GAME_WIDTH,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, true); // Right wall
    CreateWall(world, obstacle_array, GAME_WIDTH/2,              0,    GAME_WIDTH, GAME_HEIGHT/40, false); // Ceiling
}

// PLATFORM LEVEL
void Map::LoadLevel1(b2World& world, std::vector<Obstacle>& obstacle_array) {
    // Platforms
    //         World                                        posX                           posY         sizeX           sizeY  ground?
    CreateWall(world, obstacle_array, GAME_WIDTH/2 - GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/4, GAME_WIDTH/10, GAME_HEIGHT/45, true); // Upper left platform
    CreateWall(world, obstacle_array,                GAME_WIDTH/2,  GAME_HEIGHT/2 - GAME_HEIGHT/5,  GAME_WIDTH/5, GAME_HEIGHT/35, true); // Upper middle platform
    CreateWall(world, obstacle_array,                GAME_WIDTH/2,  GAME_HEIGHT/2 + GAME_HEIGHT/4,  GAME_WIDTH/5, GAME_HEIGHT/40, true); // Middle middle platform
    CreateWall(world, obstacle_array,  GAME_WIDTH - GAME_WIDTH/40,  GAME_HEIGHT/2 - GAME_HEIGHT/4,  GAME_WIDTH/4, GAME_HEIGHT/40, true); // Upper right stub
    CreateWall(world, obstacle_array,   GAME_WIDTH - GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/20,  GAME_WIDTH/7, GAME_HEIGHT/60, true); // Middle right floating
    CreateWall(world, obstacle_array,  GAME_WIDTH - GAME_WIDTH/20,    GAME_HEIGHT - GAME_HEIGHT/3,  GAME_WIDTH/9, GAME_HEIGHT/35, true); // Middle right stub
    
    //         World                                              posX                           posY         sizeX              sizeY   grnd angle friction
    CreateAngledWall(world, obstacle_array, GAME_WIDTH/2 - GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/10,  GAME_WIDTH/5, GAME_HEIGHT/50,  true,   7, 1.f); // Middle left platform
    CreateAngledWall(world, obstacle_array, GAME_WIDTH/2 + GAME_WIDTH/3,   GAME_HEIGHT - GAME_HEIGHT/40,  GAME_WIDTH/2,  GAME_HEIGHT/8,  true, 350, 5.f); // Lower right rectangle
    CreateAngledWall(world, obstacle_array, GAME_WIDTH/2 - GAME_WIDTH/3,   GAME_HEIGHT - GAME_HEIGHT/50,  GAME_WIDTH/2, GAME_HEIGHT/15,  true,   5, 5.f); // Fat lower left platform
    CreateAngledWall(world, obstacle_array,  GAME_WIDTH - GAME_WIDTH/20,                 GAME_HEIGHT/50,  GAME_WIDTH/2, GAME_HEIGHT/15, false,   8, 5.f); // Upper right angled border
 
    Spring spring;
    spring.rect.setPosition(sf::Vector2f(GAME_WIDTH/15, GAME_HEIGHT - GAME_HEIGHT/12));
    spring_array.push_back(spring);

    GenerateBorders(world, obstacle_array);
}

// ABISS LEVEL
void Map::LoadLevel2(b2World& world, std::vector<Obstacle>& obstacle_array) {

}

// TREE BRANCHES LEVEL
void Map::LoadLevel3(b2World& world, std::vector<Obstacle>& obstacle_array) {
    // Platforms
    //         World                                        posX                           posY            sizeX           sizeY  ground?
    CreateWall(world, obstacle_array,  GAME_WIDTH/2 - GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/5, GAME_WIDTH/10, GAME_HEIGHT/33, true); // Upper left platform
    CreateWall(world, obstacle_array,  GAME_WIDTH/2 + GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/5, GAME_WIDTH/10, GAME_HEIGHT/33, true); // Upper right platform
    CreateWall(world, obstacle_array,                 GAME_WIDTH/2,    GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/7, GAME_HEIGHT/25, true); // Middle lower platform
    CreateWall(world, obstacle_array,    GAME_WIDTH - GAME_WIDTH/2,                  GAME_HEIGHT/7,  GAME_WIDTH/7, GAME_HEIGHT/25, true); // Middle right platform
    CreateWall(world, obstacle_array,  GAME_WIDTH/2 - GAME_WIDTH/8,                  GAME_HEIGHT/2,  GAME_WIDTH/7, GAME_HEIGHT/25, true); // Middle left mini platform
    CreateWall(world, obstacle_array,  GAME_WIDTH/2 + GAME_WIDTH/8,                  GAME_HEIGHT/2,  GAME_WIDTH/7, GAME_HEIGHT/25, true); // Middle right mini platform

    //         World                                            posX                           posY         sizeX               sizeY   grnd angle friction
    CreateAngledWall(world, obstacle_array,              GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/6,  GAME_HEIGHT/35,  true,  10, 0.1f); // Lower left rectangle
    CreateAngledWall(world, obstacle_array, GAME_WIDTH - GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/6,  GAME_HEIGHT/35,  true, 350, 0.1f); // Lower right rectangle

    Spring spring;
    spring.rect.setPosition(sf::Vector2f(GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7 - spring.rect.getLocalBounds().height));
    spring.rect.setRotation(10);
    spring_array.push_back(spring);
    spring.rect.setPosition(sf::Vector2f(GAME_WIDTH - GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7 - spring.rect.getLocalBounds().height));
    spring.rect.setRotation(350);
    spring_array.push_back(spring);
}
