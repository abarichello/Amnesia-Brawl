#include "map.h"

Map::Map() = default;

Map::Map(std::size_t level_number, b2World& world) {
    if (!neon_texture.loadFromFile(NEON_TEXTURE)) {
        std::cout << "Error loading neon texture" << "\n";
    }
    if (!spring_texture.loadFromFile(SPRING_TEXTURE)) {
        std::cout << "Error loading spring texture" << "\n";
    }
    if (!fog_texture.loadFromFile(FOG_TEXTURE)) {
        std::cout << "Error loading fog texture" << "\n";
    }

    neon_texture.setRepeated(true);
    neon_texture.setSmooth(true);
    fog_texture.setRepeated(true);
    fog_texture.setSmooth(true);
    
    switch(level_number) {
        case 1:
            LoadLevel1(world); // NIGHT CLUB LEVEL
            break;
        case 2:
            LoadLevel2(world); // 
            break;
        case 3:
            LoadLevel3(world); // 
            break;
    }
}

void Map::Draw(sf::RenderWindow& window) {
    for (auto wall : obstacle_array) {
        wall.sprite.setPosition(SCALE * wall.body->GetPosition().x, SCALE * wall.body->GetPosition().y);
        wall.sprite.setRotation(wall.body->GetAngle() * 180 / b2_pi);
        window.draw(wall.sprite);
    }
}

void Map::CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, sf::Texture& texture) {
    // SFML settings
    Obstacle wall;
    wall.rect.setSize(sf::Vector2f(sizeX, sizeY));
    wall.rect.setPosition(posX, posY);
    wall.rect.setOrigin(wall.rect.getSize().x / 2, wall.rect.getSize().y / 2);
    wall.is_ground = is_ground;

    // Box2D settings
    wall.bodydef.type = b2_staticBody;
    wall.bodydef.position.Set(wall.rect.getPosition().x / SCALE, wall.rect.getPosition().y / SCALE);
    wall.fixturedef.shape = &wall.shape;
    wall.fixturedef.density = 0.f;
    wall.shape.SetAsBox(wall.rect.getLocalBounds().width / 2 / SCALE, wall.rect.getLocalBounds().height / 2 / SCALE);
    
    // Creating physics body
    wall.body = world.CreateBody(&wall.bodydef);
    wall.body->CreateFixture(&wall.fixturedef);

    // Sprites
    wall.sprite.setOrigin(wall.rect.getLocalBounds().width / 2, wall.rect.getLocalBounds().height / 2);
    wall.sprite.setTextureRect(sf::IntRect(0, 0, wall.rect.getLocalBounds().width, wall.rect.getLocalBounds().height));
    if (is_ground) {
        sf::Color color(50 + GenerateRandom(206), 50 + GenerateRandom(206), 50 + GenerateRandom(206));
        wall.sprite.setTexture(texture);
        wall.sprite.setColor(color);
    } else {
        wall.sprite.setTexture(texture);
        wall.sprite.setColor(sf::Color::White);
    }
    obstacle_array.push_back(wall);
}

void Map::CreateAngledWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction, sf::Texture& texture) {
    // SFML settings
    Obstacle wall;
    b2BodyDef bodydef;
    wall.rect.setSize(sf::Vector2f(sizeX, sizeY));
    wall.rect.setPosition(posX, posY);
    wall.rect.setOrigin(wall.rect.getSize().x / 2, wall.rect.getSize().y / 2);
    wall.rect.setRotation(angle);
    wall.is_ground = is_ground;

    // Box2D settings
    bodydef.type = b2_staticBody;
    bodydef.position.Set(wall.rect.getPosition().x / SCALE, wall.rect.getPosition().y / SCALE);
    bodydef.angle = angle * 0.01745329f; // Degree to radians
    wall.fixturedef.shape = &wall.shape;
    wall.fixturedef.friction = friction;
    wall.shape.SetAsBox(wall.rect.getLocalBounds().width / 2 / SCALE, wall.rect.getLocalBounds().height / 2 / SCALE);

    // Create physics body
    wall.body = world.CreateBody(&bodydef);
    wall.body->CreateFixture(&wall.fixturedef);

    // Texture
    wall.sprite.setOrigin(wall.rect.getLocalBounds().width / 2, wall.rect.getLocalBounds().height / 2);
    wall.sprite.setTextureRect(sf::IntRect(0, 0, wall.rect.getLocalBounds().width, wall.rect.getLocalBounds().height));
    if (is_ground) {
        sf::Color color(50 + GenerateRandom(206), 50 + GenerateRandom(206), 50 + GenerateRandom(206));
        wall.sprite.setTexture(neon_texture);
        wall.sprite.setColor(color);
    } else {
        wall.sprite.setTexture(texture);
        wall.sprite.setColor(sf::Color::White);
    }
    obstacle_array.push_back(wall);
}

void Map::CreateSpring(int posX, int posY, size_t angle) {
    Spring spring;
    spring.rect.setPosition(sf::Vector2f(posX, posY - spring.rect.getLocalBounds().height)); // PosY minus the spring height
    spring.rect.setRotation(angle);
    spring.sprite.setTexture(spring_texture);
    spring.sprite.setOrigin(spring.rect.getLocalBounds().width / 2, spring.rect.getLocalBounds().height / 2);
    spring.sprite.setTextureRect(sf::IntRect(0, 0, spring.rect.getLocalBounds().width, spring.rect.getLocalBounds().height));
    spring_array.push_back(spring);
}

void Map::DrawSprings(sf::RenderWindow& window) {
    for (auto spring : spring_array) {
        window.draw(spring.rect);
    }
}

void Map::ClearMap() {
    for (auto wall : obstacle_array) {
        obstacle_array.pop_back();
    }
}

void Map::GenerateBorders(b2World& world, sf::Texture& texture) {
    //         World                         posX              posY         sizeX           sizeY  ground?
    // Boundaries
    CreateWall(world, GAME_WIDTH/2,    GAME_HEIGHT,    GAME_WIDTH, GAME_HEIGHT/40,  true, texture); // Ground
    CreateWall(world,            0,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, false, texture); // Left wall
    CreateWall(world,   GAME_WIDTH,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, false, texture); // Right wall
    CreateWall(world, GAME_WIDTH/2,              0,    GAME_WIDTH, GAME_HEIGHT/40, false, texture); // Ceiling
}

// NIGHT CLUB
void Map::LoadLevel1(b2World& world) {
    // Platforms
    //         World                                        posX                           posY         sizeX           sizeY  ground?
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/4, GAME_WIDTH/10, GAME_HEIGHT/30, true, neon_texture); // Upper left platform
    CreateWall(world,                GAME_WIDTH/2,  GAME_HEIGHT/2 - GAME_HEIGHT/5,  GAME_WIDTH/5, GAME_HEIGHT/30, true, neon_texture); // Upper middle platform
    CreateWall(world,                GAME_WIDTH/2,  GAME_HEIGHT/2 + GAME_HEIGHT/4,  GAME_WIDTH/5, GAME_HEIGHT/25, true, neon_texture); // Middle middle platform
    CreateWall(world,  GAME_WIDTH - GAME_WIDTH/40,  GAME_HEIGHT/2 - GAME_HEIGHT/4,  GAME_WIDTH/4, GAME_HEIGHT/25, true, neon_texture); // Upper right stub
    CreateWall(world,   GAME_WIDTH - GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/20,  GAME_WIDTH/7, GAME_HEIGHT/25, true, neon_texture); // Middle right floating
    CreateWall(world,  GAME_WIDTH - GAME_WIDTH/20,    GAME_HEIGHT - GAME_HEIGHT/3,  GAME_WIDTH/9, GAME_HEIGHT/30, true, neon_texture); // Middle right stub
    
    //         World                                              posX                           posY         sizeX              sizeY   grnd angle friction
    CreateAngledWall(world, GAME_WIDTH/2 - GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/10,  GAME_WIDTH/5, GAME_HEIGHT/33,  true,   7, 1.f, neon_texture); // Middle left platform
    CreateAngledWall(world, GAME_WIDTH/2 + GAME_WIDTH/3,                    GAME_HEIGHT,  GAME_WIDTH/2,  GAME_HEIGHT/6,  true, 350, 5.f, neon_texture); // Lower right rectangle
    CreateAngledWall(world, GAME_WIDTH/2 - GAME_WIDTH/3,   GAME_HEIGHT - GAME_HEIGHT/50,  GAME_WIDTH/2, GAME_HEIGHT/15,  true,   5, 5.f, neon_texture); // Fat lower left platform
    CreateAngledWall(world,  GAME_WIDTH - GAME_WIDTH/20,                 GAME_HEIGHT/50,  GAME_WIDTH/2, GAME_HEIGHT/15, false,   8, 5.f, neon_texture); // Upper right angled border
 
    CreateSpring(GAME_WIDTH/15, GAME_HEIGHT - GAME_HEIGHT/18, 0);

    GenerateBorders(world, fog_texture);
}

void Map::LoadLevel2(b2World& world) {
    // TODO
}

// 
void Map::LoadLevel3(b2World& world) {
    // Platforms
    //         World                                        posX                           posY            sizeX           sizeY  ground?
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/5, GAME_WIDTH/10, GAME_HEIGHT/33, true, neon_texture); // Upper left platform
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/5, GAME_WIDTH/10, GAME_HEIGHT/33, true, neon_texture); // Upper right platform
    CreateWall(world,                GAME_WIDTH/2,    GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/7, GAME_HEIGHT/25, true, neon_texture); // Middle lower platform
    CreateWall(world,   GAME_WIDTH - GAME_WIDTH/2,                  GAME_HEIGHT/7,  GAME_WIDTH/7, GAME_HEIGHT/25, true, neon_texture); // Middle right platform
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/8,                  GAME_HEIGHT/2,  GAME_WIDTH/7, GAME_HEIGHT/25, true, neon_texture); // Middle left mini platform
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/8,                  GAME_HEIGHT/2,  GAME_WIDTH/7, GAME_HEIGHT/25, true, neon_texture); // Middle right mini platform

    //         World                                            posX                           posY         sizeX               sizeY   grnd angle friction
    CreateAngledWall(world,              GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/6,  GAME_HEIGHT/35,  true,  10, 0.1f, neon_texture); // Lower left rectangle
    CreateAngledWall(world, GAME_WIDTH - GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/6,  GAME_HEIGHT/35,  true, 350, 0.1f, neon_texture); // Lower right rectangle

    CreateSpring(GAME_WIDTH/10, GAME_HEIGHT - GAME_HEIGHT/7, 10);
    CreateSpring(GAME_WIDTH - GAME_WIDTH/10, GAME_HEIGHT - GAME_HEIGHT/7, 350);
}
