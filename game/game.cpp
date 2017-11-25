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

    // Box2D world
    CreateGround(world);
    CreatePlayers(world, GAME_WIDTH/2, GAME_HEIGHT - 200);

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
        
        // Body Iterarions
        for (b2Body* body_iter = world.GetBodyList(); body_iter != NULL; body_iter = body_iter->GetNext()) {
            if (body_iter->GetType() == b2_dynamicBody) {
                player1->rect.setPosition(SCALE * body_iter->GetPosition().x, SCALE * body_iter->GetPosition().y);
                player1->rect.setRotation(body_iter->GetAngle() * 180/b2_pi);
            } else if (body_iter->GetType() == b2_staticBody) {
                ground.rect.setPosition(SCALE * body_iter->GetPosition().x, SCALE * body_iter->GetPosition().y);
                ground.rect.setRotation(body_iter->GetAngle() * 180/b2_pi);
                window.draw(ground.rect);
            }
        }

        // Update players
        for (auto player : player_array) {
            player->Update(elapsed_time);
            window.draw(player->rect);
        }
    
        window.display();
    }
}

void Game::LoadResources() {
    
}


void Game::CreateGround(b2World& world) {
    ground.rect.setSize(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT/20));
    ground.rect.setPosition(0, GAME_HEIGHT - ground.rect.getLocalBounds().height);
    
    ground.bodydef.type = b2_staticBody;
    ground.bodydef.position.Set(ground.rect.getPosition().x/SCALE, ground.rect.getPosition().y/SCALE);
    ground.shape.SetAsBox((ground.rect.getLocalBounds().width)/SCALE, (ground.rect.getLocalBounds().height/2)/SCALE);
    ground.fixturedef.shape = &ground.shape;
    ground.fixturedef.density = 1.f;
    ground.fixturedef.friction = 0.1f;

    ground.body = world.CreateBody(&ground.bodydef);
    ground.body->CreateFixture(&ground.fixturedef);

    obstacle_array.push_back(ground);
}

void Game::CreatePlayers(b2World& world, int x, int y) {
    player1 = new Player();
    player1->bodydef.type = b2_dynamicBody;
    player1->bodydef.position.Set(x/SCALE, y/SCALE);
    player1->shape.SetAsBox((8/2)/SCALE, (8/2)/SCALE);
    player1->fixturedef.shape = &player1->shape;
    player1->fixturedef.density = 1.f;

    player1->body = world.CreateBody(&player1->bodydef);
    player1->body->CreateFixture(&player1->fixturedef);

    player_array.push_back(player1);
}
