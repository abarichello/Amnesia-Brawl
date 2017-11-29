#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA GAME"),
    gravity(0.f, 18.f),
    world(gravity) {

    LoadPlayer1();
    LoadPlayer2();
    LoadResources();
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
        std::map<std::string, Player*>::const_iterator itr = _game_object_manager._game_objects.begin();
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
        if (player1->rectB.getGlobalBounds().intersects(player2->rectA.getGlobalBounds()) && player1->alive) {
            player2->alive = false;
            std::cout << "rip p2" << "\n";
            _game_object_manager.Remove("Player2");
            LoadPlayer2();
        }
        if (player2->rectB.getGlobalBounds().intersects(player1->rectA.getGlobalBounds()) && player2->alive) {
            player1->alive = false;
            std::cout << "rip p1" << "\n";
            _game_object_manager.Remove("Player1");
            LoadPlayer1();
        }

        window.display();
    }
}

void Game::LoadPlayer1() {
    player1 = new Player();
    player1->name = "Player1";
    player1->jump = sf::Keyboard::Key::W;
    player1->left = sf::Keyboard::Key::A;
    player1->right = sf::Keyboard::Key::D;
    CreatePlayers(world, player1, GAME_WIDTH/2, GAME_HEIGHT - 200);
}

void Game::LoadPlayer2() {
    player2 = new Player();
    player2->name = "Player2";
    player2->rect.setFillColor(sf::Color::Red);
    player2->jump = sf::Keyboard::Key::Up;
    player2->left = sf::Keyboard::Key::Left;
    player2->right = sf::Keyboard::Key::Right;
    CreatePlayers(world, player2, GAME_WIDTH/4, GAME_HEIGHT - 200);
}

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
    CreateWall(world,  GAME_WIDTH/2 - GAME_WIDTH/3, GAME_HEIGHT/2 - GAME_HEIGHT/4, GAME_WIDTH/10, GAME_HEIGHT/45, true); // Lower right platform
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

void Game::CreatePlayers(b2World& world, Player* player, int x, int y) {
    const float density = 1;
    player->bodydef.type = b2_dynamicBody;
    player->bodydef.position.Set(x/SCALE, y/SCALE);
    player->body = world.CreateBody(&player->bodydef);

    // Width and height subtracted by one, so the rect can intersect with the ground
    player->shape.SetAsBox((23)/SCALE, (23)/SCALE); // Upper collision
    player->body->CreateFixture(&player->shape, density);

    _game_object_manager.Add(player->name, player);
}

GameObjectManager Game::_game_object_manager;
