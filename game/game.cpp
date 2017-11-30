#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA GAME"),
    gravity(0.f, 18.f),
    world(gravity),
    hud(4) {
    amnesia_blue = sf::Color(24, 165, 235);
    amnesia_red = sf::Color(227, 46, 18);
    amnesia_dark_blue = sf::Color(0, 102, 255);
    amnesia_dark_red = sf::Color(159, 0, 0);

    LoadResources();
    LoadPlayers(4);
}

void Game::Start() {
    window.setFramerateLimit(70);
    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // Delta time between frames
        sf::Time elapsed_time = global_clock.restart();

        world.Step(1.f/60.f, 10, 10);
        window.clear();

        // Update players
        std::map<std::size_t, Player*>::const_iterator itr = _game_object_manager._game_objects.begin();
        while (itr != _game_object_manager._game_objects.end()) {
            itr->second->rect.setPosition(SCALE * itr->second->body->GetPosition().x, SCALE * itr->second->body->GetPosition().y);
            itr->second->rect.setRotation(itr->second->body->GetAngle() * 180/b2_pi);
            itr->second->Update(elapsed_time, obstacle_array);
            itr->second->Draw(window);
            itr++;
        }

        // Update walls
        for (auto wall : obstacle_array) {
            wall.rect.setPosition(SCALE * wall.body->GetPosition().x, SCALE * wall.body->GetPosition().y);
            wall.rect.setRotation(wall.body->GetAngle() * 180/b2_pi);
            window.draw(wall.rect);
        }

        // Player-Player collision
        // if (player1->rectB.getGlobalBounds().intersects(player2->rectA.getGlobalBounds()) && player1->alive) {
        //     hud.p1_score += 1;
        //     player1->jumps_remaining += 1;
        //     player2->Respawn();
        //     _game_object_manager.Remove(2);
        // }
        // if (player2->rectB.getGlobalBounds().intersects(player1->rectA.getGlobalBounds()) && player2->alive) {
        //     hud.p2_score += 1;
        //     player2->jumps_remaining += 1;
        //     _game_object_manager.Remove(1);
        //     player1->Respawn();
        // }

        std::map<std::size_t, Player*>::const_iterator iter = _game_object_manager._game_objects.begin();
        while (iter != _game_object_manager._game_objects.end()) {
            std::map<std::size_t, Player*>::const_iterator iter2 = _game_object_manager._game_objects.begin();
            while (iter2 != _game_object_manager._game_objects.end()) {
                if (iter->second->rectB.getGlobalBounds().intersects(iter2->second->rectA.getGlobalBounds())) {
                    auto killer_number = iter->second->number;
                    auto killed_number = iter2->second->number;
                    switch (killer_number) {
                        case 1:
                            hud.p1_score += 1;
                            break;
                        case 2:
                            hud.p2_score += 1;
                            break;
                        case 3:
                            hud.p3_score += 1;
                            break;
                        case 4:
                            hud.p4_score += 1;
                            break;
                    }
                    iter->second->jumps_remaining = 1;
                    // _game_object_manager.Remove(killed_number);
                    iter2->second->Respawn();
                }
                ++iter2;
            }
            ++iter;
        }

        hud.Update();
        hud.Draw(window);
        window.display();
    }
}

void Game::LoadPlayers(std::size_t number_of_players) {
    player1 = new Player();
    player2 = new Player();
    SpawnPlayer(1, player1, amnesia_blue, sf::Keyboard::Key::W, sf::Keyboard::Key::A, sf::Keyboard::Key::D);
    SpawnPlayer(2, player2, amnesia_red, sf::Keyboard::Key::Up, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right);
    if (number_of_players == 4) {
        player3 = new Player();
        player4 = new Player();
        SpawnPlayer(3, player3, amnesia_dark_blue, sf::Keyboard::Key::I, sf::Keyboard::Key::J, sf::Keyboard::Key::L);
        SpawnPlayer(4, player4, amnesia_dark_red, sf::Keyboard::Key::T, sf::Keyboard::Key::F, sf::Keyboard::Key::H);
    }
}

void Game::SpawnPlayer(std::size_t number, Player* player, sf::Color color, sf::Keyboard::Key jump, sf::Keyboard::Key left, sf::Keyboard::Key right) {
    player->number = number;
    player->jump = jump;
    player->left = left;
    player->right = right;
    player->rect.setFillColor(color);

    int random_spawn = GenerateRandom(spawn_locations.size() - 1);
    b2Vec2 spawn_pos = spawn_locations[random_spawn];
    CreatePlayer(world, player, GenerateRandom(GAME_WIDTH), GenerateRandom(GAME_HEIGHT));
}

// Loads the map platforms and boundariess
void Game::LoadResources() {
    //         World                         posX                         posY          sizeX           sizeY  ground?
    // Boundaries
    CreateWall(world,                 GAME_WIDTH/2,                   GAME_HEIGHT,    GAME_WIDTH, GAME_HEIGHT/40, true); // Ground
    CreateWall(world,                            0,                 GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, true); // Left wall
    CreateWall(world,                   GAME_WIDTH,                 GAME_HEIGHT/2, GAME_WIDTH/40,    GAME_HEIGHT, true); // Right wall
    CreateWall(world,                 GAME_WIDTH/2,                             0,    GAME_WIDTH, GAME_HEIGHT/40, false); // Ceiling

    // Platforms
    CreateWall(world,                 GAME_WIDTH/4,   GAME_HEIGHT - GAME_HEIGHT/6,  GAME_WIDTH/4, GAME_HEIGHT/40, true); // Lower left
    CreateWall(world,   GAME_WIDTH - GAME_WIDTH/20,   GAME_HEIGHT - GAME_HEIGHT/3,  GAME_WIDTH/9, GAME_HEIGHT/35, true); // Lower right
    CreateWall(world,  GAME_WIDTH/2 - GAME_WIDTH/3, GAME_HEIGHT/2 - GAME_HEIGHT/4, GAME_WIDTH/10, GAME_HEIGHT/45, true); // Upper platform
    CreateWall(world,    GAME_WIDTH - GAME_WIDTH/2,                 GAME_HEIGHT/2,  GAME_WIDTH/3, GAME_HEIGHT/40, true); // Lower right

    spawn_locations.push_back(b2Vec2(               GAME_WIDTH/4,   GAME_HEIGHT - GAME_HEIGHT/6));
    spawn_locations.push_back(b2Vec2( GAME_WIDTH - GAME_WIDTH/20,   GAME_HEIGHT - GAME_HEIGHT/3));
    spawn_locations.push_back(b2Vec2(GAME_WIDTH/2 - GAME_WIDTH/3, GAME_HEIGHT/2 - GAME_HEIGHT/4));
    spawn_locations.push_back(b2Vec2(  GAME_WIDTH - GAME_WIDTH/2,                 GAME_HEIGHT/2));
}

void Game::CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground) {
    wall.rect.setSize(sf::Vector2f(sizeX, sizeY));
    wall.rect.setPosition(posX, posY);
    wall.rect.setOrigin(wall.rect.getSize().x/2, wall.rect.getSize().y/2);
    wall.is_ground = is_ground;

    wall.bodydef.type = b2_staticBody;
    wall.bodydef.position.Set(wall.rect.getPosition().x/SCALE, wall.rect.getPosition().y/SCALE);
    wall.fixturedef.shape = &wall.shape;
    wall.fixturedef.density = 0.f;
    wall.shape.SetAsBox(wall.rect.getLocalBounds().width/2/SCALE, wall.rect.getLocalBounds().height/2/SCALE);

    wall.body = world.CreateBody(&wall.bodydef);
    wall.body->CreateFixture(&wall.fixturedef);
    obstacle_array.push_back(wall);
}

void Game::CreateBall(b2World& world, int posX, int posY, int sizeX, int sizeY, bool is_ground) {
    
}

void Game::CreatePlayer(b2World& world, Player* player, int x, int y) {
    const float density = 1;
    player->bodydef.type = b2_dynamicBody;
    player->bodydef.position.Set(x/SCALE, y/SCALE);
    player->body = world.CreateBody(&player->bodydef);

    // Width and height subtracted by one, so the rect can intersect with the ground
    player->shape.SetAsBox((23)/SCALE, (23)/SCALE); // Upper collision
    player->body->CreateFixture(&player->shape, density);

    _game_object_manager.Add(player->number, player);
}

GameObjectManager Game::_game_object_manager;
