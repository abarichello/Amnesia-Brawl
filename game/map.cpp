#include "map.h"

Map::Map() = default;

Map::Map(std::size_t level_number, b2World& world) {
    if (!neon_texture.loadFromFile(NEON_TEXTURE)) {
        std::cout << "Error loading neon texture" << "\n";
    }
    if (!desert_block_texture.loadFromFile(DESERT_BLOCK_TEXTURE)) {
        std::cout << "Error loading desert block texture" << "\n";
    }
    if (!spring_texture.loadFromFile(SPRING_TEXTURE)) {
        std::cout << "Error loading spring texture" << "\n";
    }
    if (!fog_texture.loadFromFile(FOG_TEXTURE)) {
        std::cout << "Error loading fog texture" << "\n";
    }
    if (!amnesia_texture.loadFromFile(AMNESIA_LOGO)) {
        std::cout << "Error loading amnesia logo" << "\n";
    }
    if (!package_texture.loadFromFile(PACKAGE_TEXTURE)) {
        std::cout << "Error loading package texture" << "\n";
    }
    if (!teleport_texture.loadFromFile(TELEPORT_TEXTURE)) {
        std::cout << "Error loading arrow texture" << "\n";
    }

    amnesia_texture.setSmooth(true);
    amnesia_logo.setTexture(amnesia_texture);
    amnesia_logo.setScale(0.3f, 0.3f);
    amnesia_logo.setOrigin(amnesia_texture.getSize().x / 2, amnesia_texture.getSize().y / 2);
    amnesia_logo.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 3);

    neon_texture.setRepeated(true);
    neon_texture.setSmooth(true);
    desert_block_texture.setRepeated(true);
    fog_texture.setRepeated(true);
    fog_texture.setSmooth(true);
    package_texture.setRepeated(true);
    package_texture.setSmooth(true);

    this->level_number = level_number;

    switch(level_number) {
        case 1:
            LoadLevel1(world); // NIGHT CLUB LEVEL
            break;
        case 2:
            LoadLevel2(world); // 
            break;
        case 3:
            LoadLevel3(world); // TEQUILA DESERT LEVEL
            break;
    }
}

Map::~Map() {
    for (auto wall : obstacle_array) {
        obstacle_array.pop_back();
        wall.body->GetWorld()->DestroyBody(wall.body);
    }
    for (auto spr : spring_array) {
        spring_array.pop_back();
    }
    for (auto tp : tpad_array) {
        tpad_array.pop_back();
    }
}

void Map::Update() {
    for (auto wall : obstacle_array) {
        // Teleport walls outside map (falling boxes)
        if (wall.body->GetPosition().y * SCALE > GAME_HEIGHT + 200) {
            wall.body->SetLinearVelocity(b2Vec2(0, 0));
            if (RandomBool()) { // Randomize left or right spawn
                wall.body->SetTransform(b2Vec2((GAME_WIDTH / 2 - GAME_WIDTH/9 - GenerateRandom(150)) / SCALE, -20 / SCALE), 0);
            } else {
                wall.body->SetTransform(b2Vec2((GAME_WIDTH / 2 + GAME_WIDTH/9 + GenerateRandom(150)) / SCALE, 0), -20 / SCALE);
            }
        }
    }

    // Pulse background color
    if (level_number == 1) {
        pulse_variable += 1;
        background_sprite.setColor(sf::Color(pulse_variable, pulse_variable, pulse_variable));
    }
}

void Map::Draw(sf::RenderWindow& window) {
    window.draw(background_sprite);
    window.draw(amnesia_logo);
    for (auto wall : obstacle_array) {
        // Sprite drawing
        wall.sprite.setPosition(SCALE * wall.body->GetPosition().x, SCALE * wall.body->GetPosition().y);
        wall.sprite.setRotation(wall.body->GetAngle() * 180 / b2_pi);
        window.draw(wall.sprite);
    }
}

void Map::CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground, size_t angle, float friction, sf::Texture& texture, sf::Color color = sf::Color::White, b2BodyType type = b2_staticBody) {
    // SFML settings
    Obstacle wall;
    b2BodyDef bodydef;
    wall.rect.setSize(sf::Vector2f(sizeX, sizeY));
    wall.rect.setPosition(posX, posY);
    wall.rect.setOrigin(wall.rect.getSize().x / 2, wall.rect.getSize().y / 2);
    wall.rect.setRotation(angle);
    wall.is_ground = is_ground;

    // Box2D settings
    bodydef.type = type;
    bodydef.position.Set(wall.rect.getPosition().x / SCALE, wall.rect.getPosition().y / SCALE);
    bodydef.angle = angle * 0.01745329f; // Degree to radians
    wall.fixturedef.shape = &wall.shape;
    wall.fixturedef.friction = friction;
    wall.fixturedef.density = 30;
    wall.shape.SetAsBox(wall.rect.getLocalBounds().width / 2 / SCALE, wall.rect.getLocalBounds().height / 2 / SCALE);

    // Create physics body
    wall.body = world.CreateBody(&bodydef);
    wall.body->CreateFixture(&wall.fixturedef);

    // Texture
    wall.sprite.setOrigin(wall.rect.getLocalBounds().width / 2, wall.rect.getLocalBounds().height / 2);
    wall.sprite.setTextureRect(sf::IntRect(0, 0, wall.rect.getLocalBounds().width, wall.rect.getLocalBounds().height));
    wall.sprite.setTexture(texture);
    wall.sprite.setColor(color);
    obstacle_array.push_back(wall);
}

void Map::CreateSpring(int posX, int posY, size_t angle, sf::Color color = sf::Color::White) {
    Spring spring;
    spring.rect.setPosition(sf::Vector2f(posX, posY));
    spring.rect.setRotation(angle);
    spring.sprite.setTexture(spring_texture);
    spring.sprite.setColor(color);
    spring_array.push_back(spring);
}

void Map::CreateTeleport(int posX, int posY, b2Vec2 destination, sf::Color color = sf::Color::White) {
    TeleportPad tpad;
    tpad.SetDestination(destination);
    tpad.rect.setPosition(sf::Vector2f(posX, posY));
    tpad.sprite.setTexture(teleport_texture);
    tpad.sprite.setColor(color);
    tpad_array.push_back(tpad);
}

void Map::GenerateBorders(b2World& world, sf::Texture& texture) {
    //         World        posX              posY         sizeX        sizeY  ground?  angle friction
    // Boundaries
    CreateWall(world, GAME_WIDTH/2,    GAME_HEIGHT,    GAME_WIDTH, GAME_HEIGHT/40,  true, 0, 1.f, texture, sf::Color::White); // Ground
    CreateWall(world,            0,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, false, 0, 1.f, texture, sf::Color::White); // Left wall
    CreateWall(world,   GAME_WIDTH,  GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, false, 0, 1.f, texture, sf::Color::White); // Right wall
    CreateWall(world, GAME_WIDTH/2,              0,    GAME_WIDTH, GAME_HEIGHT/40, false, 0, 1.f, texture, sf::Color::White); // Ceiling
}

// NIGHT CLUB
void Map::LoadLevel1(b2World& world) {
    // Background
    degrade.create(GAME_WIDTH, GAME_HEIGHT);
    for (auto i = 0; i < GAME_WIDTH; ++i) {
        for (auto j = 0; j < GAME_HEIGHT; ++j) {
            sf::Color color(256, 256, i / 9);
            degrade.setPixel(i, j, color);
        }
    }
    background_texture.loadFromImage(degrade);
    background_sprite.setTexture(background_texture);

    // Platforms
    //         World                         posX                           posY          sizeX           sizeY  ground? angle friction
    sf::Color color(100 + GenerateRandom(100), 100 + GenerateRandom(100), 100 + GenerateRandom(100));
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/4, GAME_WIDTH/10, GAME_HEIGHT/30, true, 0, 1.f, neon_texture, color); // Upper left platform
    color = sf::Color(100 + GenerateRandom(100), 100 + GenerateRandom(100), 100 + GenerateRandom(100));
    CreateWall(world,                GAME_WIDTH/2,  GAME_HEIGHT/2 - GAME_HEIGHT/5,  GAME_WIDTH/5, GAME_HEIGHT/30, true, 0, 1.f, neon_texture, color); // Upper middle platform
    color = sf::Color(100 + GenerateRandom(100), 100 + GenerateRandom(100), 100 + GenerateRandom(100));
    CreateWall(world,                GAME_WIDTH/2,  GAME_HEIGHT/2 + GAME_HEIGHT/4,  GAME_WIDTH/5, GAME_HEIGHT/25, true, 0, 1.f, neon_texture, color); // Middle middle platform
    color = sf::Color(100 + GenerateRandom(100), 100 + GenerateRandom(100), 100 + GenerateRandom(100));
    CreateWall(world,  GAME_WIDTH - GAME_WIDTH/40,  GAME_HEIGHT/2 - GAME_HEIGHT/4,  GAME_WIDTH/4, GAME_HEIGHT/25, true, 0, 1.f, neon_texture, color); // Upper right stub
    color = sf::Color(100 + GenerateRandom(100), 100 + GenerateRandom(100), 100 + GenerateRandom(100));
    CreateWall(world,   GAME_WIDTH - GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/20,  GAME_WIDTH/7, GAME_HEIGHT/25, true, 0, 1.f, neon_texture, color); // Middle right floating
    color = sf::Color(100 + GenerateRandom(100), 100 + GenerateRandom(100), 100 + GenerateRandom(100));
    CreateWall(world,  GAME_WIDTH - GAME_WIDTH/20,    GAME_HEIGHT - GAME_HEIGHT/3,  GAME_WIDTH/9, GAME_HEIGHT/30, true, 0, 1.f, neon_texture, color); // Middle right stub
    
    //               World                        posX                             posY          sizeX           sizeY   grnd angle friction
    color = sf::Color(100 + GenerateRandom(100), 100 + GenerateRandom(100), 100 + GenerateRandom(100));
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/10,  GAME_WIDTH/5, GAME_HEIGHT/33,  true,   7, 5.f, neon_texture, color); // Middle left platform
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/3,                    GAME_HEIGHT,  GAME_WIDTH/2,  GAME_HEIGHT/6,  true, 350, 5.f, neon_texture, color); // Lower right rectangle
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/3,   GAME_HEIGHT - GAME_HEIGHT/50,  GAME_WIDTH/2, GAME_HEIGHT/15,  true,   5, 5.f, neon_texture, color); // Fat lower left platform
    CreateWall(world,  GAME_WIDTH - GAME_WIDTH/20,                 GAME_HEIGHT/50,  GAME_WIDTH/2, GAME_HEIGHT/15, false,   8, 5.f, neon_texture, color); // Upper right angled border
 
    CreateSpring(GAME_WIDTH/15, GAME_HEIGHT - GAME_HEIGHT/11, 0);

    GenerateBorders(world, fog_texture);
}

void Map::LoadLevel2(b2World& world) {
    // Background image generation
    degrade.create(GAME_WIDTH, GAME_HEIGHT);
    for (auto i = 0; i < GAME_WIDTH; ++i) {
        for (auto j = 0; j < GAME_HEIGHT; ++j) {
            // sf::Color color(GenerateRandom(75), GenerateRandom(75), GenerateRandom(75);
            sf::Color color(75, 75, 75);
            degrade.setPixel(i, j, color);
        }
    }
    background_texture.loadFromImage(degrade);
    background_sprite.setTexture(background_texture);

    //         World                        posX                            posY          sizeX           sizeY   grnd   angle friction
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/10,  GAME_WIDTH/5, GAME_HEIGHT/35,  true,  200, 0.1f, neon_texture, sf::Color::White); // Middle left wall
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/4, GAME_HEIGHT/2 + GAME_HEIGHT/10,  GAME_WIDTH/5, GAME_HEIGHT/35,  true,  340, 0.1f, neon_texture, sf::Color::White); // Middle right wall

    //               World                    posX            posY          sizeX           sizeY grnd   angle friction
    CreateWall(world,   GAME_WIDTH/2 - GAME_WIDTH/6, GAME_HEIGHT/3, GAME_WIDTH/10, GAME_HEIGHT/45, true, 200, 1.f, neon_texture, sf::Color::Red); // Top left wall platform (object collector)
    CreateWall(world,   GAME_WIDTH/2 + GAME_WIDTH/6, GAME_HEIGHT/3, GAME_WIDTH/10, GAME_HEIGHT/45, true, 340, 1.f, neon_texture, sf::Color::Red); // Top right wall platform (object collector)
    CreateWall(world,                 GAME_WIDTH/20, GAME_HEIGHT/3, GAME_WIDTH/10, GAME_HEIGHT/45, true,   3, 1.f, neon_texture, sf::Color::Red); // Middle wall platform
    CreateWall(world,   GAME_WIDTH -  GAME_WIDTH/20, GAME_HEIGHT/3, GAME_WIDTH/10, GAME_HEIGHT/45, true, 357, 1.f, neon_texture, sf::Color::Red); // Middle wall platform

    //         World                        posX    posY                  sizeX                  sizeY   grnd   angle friction
    // Falling obstacle boxes
    for (auto i = 0u; i < 4; ++i) {
        CreateWall(world, GAME_WIDTH / 2 - GAME_WIDTH/9, 0, 30 + GenerateRandom(25), 30 + GenerateRandom(25), true, GenerateRandom(365), 0.1f, package_texture, sf::Color::White, b2_dynamicBody);
    }
    for (auto j = 0u; j < 4; ++j) {
        CreateWall(world, GAME_WIDTH / 2 + GAME_WIDTH/9, 0, 30 + GenerateRandom(25), 30 + GenerateRandom(25), true, GenerateRandom(365), 0.1f, package_texture, sf::Color::White, b2_dynamicBody);
    }

    // Create other objects
    CreateSpring(             GAME_WIDTH/9, GAME_HEIGHT - GAME_HEIGHT/15,   5);
    CreateSpring(GAME_WIDTH - GAME_WIDTH/9, GAME_HEIGHT - GAME_HEIGHT/15, 355);
    //                                  posX                          posY                               destination
    CreateTeleport(             GAME_WIDTH/80,               GAME_HEIGHT/5, b2Vec2((GAME_WIDTH - GAME_WIDTH/15) / SCALE, (GAME_HEIGHT/5)/ SCALE)); // Upper left teleport
    CreateTeleport(GAME_WIDTH - GAME_WIDTH/80,               GAME_HEIGHT/5, b2Vec2((GAME_WIDTH/15) / SCALE, (GAME_HEIGHT/4)/ SCALE)); // Upper right teleport
    CreateTeleport(             GAME_WIDTH/80, GAME_HEIGHT - GAME_HEIGHT/7, b2Vec2((GAME_WIDTH - GAME_WIDTH/15) / SCALE, (GAME_HEIGHT - GAME_HEIGHT/7)/ SCALE)); // Lower left teleport
    CreateTeleport(GAME_WIDTH - GAME_WIDTH/80, GAME_HEIGHT - GAME_HEIGHT/7, b2Vec2((GAME_WIDTH/15) / SCALE, (GAME_HEIGHT - GAME_HEIGHT/7)/ SCALE)); // Lower right teleport

    // Boundaries
    //         World                         posX             posY           sizeX           sizeY  ground? angle friction
    CreateWall(world,                GAME_WIDTH/2,           -45,    GAME_WIDTH,  GAME_HEIGHT/40, false, 0, 1.f, fog_texture); // Ceiling Shield
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/3,             0,  GAME_WIDTH/6,  GAME_HEIGHT/40, false, 0, 1.f, fog_texture); // Ceiling left
    CreateWall(world,                GAME_WIDTH/2,             0,  GAME_WIDTH/6,  GAME_HEIGHT/40, false, 0, 1.f, fog_texture); // Ceiling middle
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/3,             0,  GAME_WIDTH/6,  GAME_HEIGHT/40, false, 0, 1.f, fog_texture); // Ceiling right

    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/3, GAME_HEIGHT-5,  GAME_WIDTH/3,  GAME_HEIGHT/20,  true,   5, 0.05f, fog_texture); // Ground left
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/3, GAME_HEIGHT-5,  GAME_WIDTH/3,  GAME_HEIGHT/20,  true, 355, 0.05f, fog_texture); // Ground right

    CreateWall(world,                           0, GAME_HEIGHT/2, GAME_WIDTH/40,     GAME_HEIGHT, false, 0, 1.f, fog_texture); // Left wall
    CreateWall(world,                  GAME_WIDTH, GAME_HEIGHT/2, GAME_WIDTH/40,     GAME_HEIGHT, false, 0, 1.f, fog_texture); // Right wall
}

// TEQUILA DESERT
void Map::LoadLevel3(b2World& world) {
    background_texture.loadFromFile(DESERT_TEXTURE);
    background_sprite.setTexture(background_texture);

    // Platforms
    //         World                        posX                            posY          sizeX           sizeY ground?    texture    color
    sf::Color desert_color = sf::Color(255, 205, 0);
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/5, GAME_WIDTH/10, GAME_HEIGHT/33,  true, 0, 1.f, desert_block_texture, desert_color); // Upper left platform
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/3,  GAME_HEIGHT/2 - GAME_HEIGHT/5, GAME_WIDTH/10, GAME_HEIGHT/33,  true, 0, 1.f, desert_block_texture, desert_color); // Upper right platform
    CreateWall(world,                GAME_WIDTH/2,    GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/7, GAME_HEIGHT/25,  true, 0, 1.f, desert_block_texture, desert_color); // Middle lower platform
    CreateWall(world,   GAME_WIDTH - GAME_WIDTH/2,                  GAME_HEIGHT/7,  GAME_WIDTH/7, GAME_HEIGHT/25,  true, 0, 1.f, desert_block_texture, desert_color); // Middle right platform
    CreateWall(world, GAME_WIDTH/2 - GAME_WIDTH/8,                  GAME_HEIGHT/2,  GAME_WIDTH/7, GAME_HEIGHT/25,  true, 0, 1.f, desert_block_texture, desert_color); // Middle left mini platform
    CreateWall(world, GAME_WIDTH/2 + GAME_WIDTH/8,                  GAME_HEIGHT/2,  GAME_WIDTH/7, GAME_HEIGHT/25,  true, 0, 1.f, desert_block_texture, desert_color); // Middle right mini platform
    CreateWall(world,                GAME_WIDTH/2,                 -GAME_HEIGHT/3,    GAME_WIDTH, GAME_HEIGHT/25, false, 0, 1.f, desert_block_texture, desert_color); // Upper platform outside screen

    //         World                             posX                           posY         sizeX              sizeY   grnd angle friction
    sf::Color color2 = sf::Color(200, 200, 0);
    CreateWall(world,              GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/6,  GAME_HEIGHT/35,  true,  10, 2.f, desert_block_texture, color2); // Lower left rectangle
    CreateWall(world, GAME_WIDTH - GAME_WIDTH/10,   GAME_HEIGHT - GAME_HEIGHT/7,  GAME_WIDTH/6,  GAME_HEIGHT/35,  true, 350, 2.f, desert_block_texture, color2); // Lower right rectangle

    CreateSpring(GAME_WIDTH/10, GAME_HEIGHT - GAME_HEIGHT/5.5f, 10);
    CreateSpring(GAME_WIDTH - GAME_WIDTH/10, GAME_HEIGHT - GAME_HEIGHT/5.5f, 350);
}
