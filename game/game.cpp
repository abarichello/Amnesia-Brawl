#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA GAME"),
    gravity(0.f, 18.f),
    world(gravity) {
    LoadResources();
    Start();
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
        for (auto player : player_array) {
            player->rect.setPosition(SCALE * player->body->GetPosition().x, SCALE * player->body->GetPosition().y);
            player->rect.setRotation(player->body->GetAngle() * 180/b2_pi);
            player->Update(elapsed_time, obstacle_array[0]);
            window.draw(player->rect);
        }

        // Update walls
        for (auto wall : obstacle_array) {
            wall.rect.setPosition(SCALE * wall.body->GetPosition().x, SCALE * wall.body->GetPosition().y);
            wall.rect.setRotation(wall.body->GetAngle() * 180/b2_pi);
            window.draw(wall.rect);
        }
    
        window.display();
    }
}

void Game::LoadResources() {
    player1 = new Player();
    player1->jump = sf::Keyboard::Key::W;
    player1->left = sf::Keyboard::Key::A;
    player1->right = sf::Keyboard::Key::D;

    player2 = new Player();
    player2->rect.setFillColor(sf::Color::Red);
    player2->jump = sf::Keyboard::Key::Up;
    player2->left = sf::Keyboard::Key::Left;
    player2->right = sf::Keyboard::Key::Right;

    CreatePlayers(world, player1, GAME_WIDTH/2, GAME_HEIGHT - 200);
    CreatePlayers(world, player2, GAME_WIDTH/4, GAME_HEIGHT - 200);

    CreateWall(world, GAME_WIDTH/2, GAME_HEIGHT, GAME_WIDTH, GAME_HEIGHT/40); // Ground
    CreateWall(world, 0, GAME_HEIGHT/2, GAME_WIDTH/40, GAME_HEIGHT); // Left wall
    CreateWall(world, GAME_WIDTH, GAME_HEIGHT/2, GAME_WIDTH/40, GAME_HEIGHT); // Right wall
    CreateWall(world, GAME_WIDTH/2, 0, GAME_WIDTH, GAME_HEIGHT/40); // Ceiling
}

void Game::CreateWall(b2World& world, int posX, int posY, int sizeX, int sizeY) {
    wall.rect.setSize(sf::Vector2f(sizeX, sizeY));
    wall.rect.setPosition(posX, posY);
    wall.rect.setOrigin(wall.rect.getSize().x/2, wall.rect.getSize().y/2);
    
    wall.bodydef.type = b2_staticBody;
    wall.bodydef.position.Set(wall.rect.getPosition().x/SCALE, wall.rect.getPosition().y/SCALE);
    wall.shape.SetAsBox(wall.rect.getLocalBounds().width/2/SCALE, wall.rect.getLocalBounds().height/2/SCALE);
    wall.fixturedef.shape = &wall.shape;
    wall.fixturedef.filter.categoryBits = 2;
    wall.fixturedef.density = 0.f;

    wall.body = world.CreateBody(&wall.bodydef);
    wall.body->CreateFixture(&wall.fixturedef);

    obstacle_array.push_back(wall);
}

void Game::CreatePlayers(b2World& world, Player* player, int x, int y) {
    player->bodydef.type = b2_dynamicBody;
    player->bodydef.position.Set(x/SCALE, y/SCALE);
    
    // Width and height subtracted by one, so the rect can intersect with the ground
    player->shape.SetAsBox((player->rect.getLocalBounds().width/2-1)/SCALE, (player->rect.getLocalBounds().width/2-1)/SCALE);
    player->fixturedef.shape = &player1->shape;
    player->fixturedef.filter.categoryBits = 2;
    player->fixturedef.density = 1.f;

    player->body = world.CreateBody(&player->bodydef);
    player->body->CreateFixture(&player->fixturedef);

    player_array.push_back(player);
}
