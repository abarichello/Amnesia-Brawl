#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA GAME"),
    gravity(0.f, 9.8f),
    world(gravity) {
    LoadResources();
    Start();
}

void Game::Start() {
    window.setFramerateLimit(70);

    // Box2D world
    CreateGround(world);
    CreatePlayers(world, 10.f, 10.f);

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
        world.Step(elapsed_time.asSeconds(), 10, 8);
        window.clear();
        
        // Body Iterarions
        for (b2Body* body_iter = world.GetBodyList(); body_iter != NULL; body_iter = body_iter->GetNext()) {
            if (body_iter->GetType() == b2_dynamicBody) {
                player1->rect.setPosition(SCALE * body_iter->GetPosition().x, SCALE * body_iter->GetPosition().y);
                player1->rect.setRotation(body_iter->GetAngle() * 180/b2_pi);
            } else {
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
    ground.rect.setSize(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT/17));
    ground.rect.setPosition(0, GAME_HEIGHT - ground.rect.getLocalBounds().height);
    
    ground.bodydef.type = b2_staticBody;
    ground.bodydef.position = b2Vec2(ground.rect.getPosition().x/SCALE, ground.rect.getPosition().y/SCALE);
    ground.shape.SetAsBox((ground.rect.getLocalBounds().width/2)/SCALE, (ground.rect.getLocalBounds().height/2)/SCALE);
    ground.fixturedef.shape = &ground.shape;
    ground.fixturedef.density = 0.f;

    ground.body = world.CreateBody(&ground.bodydef);
    ground.body->CreateFixture(&ground.fixturedef);

    obstacle_array.push_back(ground);
}

void Game::CreatePlayers(b2World& world, int x, int y) {
    player1 = new Player();
    player1->bodydef.type = b2_dynamicBody;
    player1->bodydef.position = b2Vec2(x/SCALE, y/SCALE);
    player1->shape.SetAsBox((x/2)/SCALE, (y/2)/SCALE);
    player1->fixturedef.shape = &player1->shape;
    player1->fixturedef.density = 1.f;

    player1->body = world.CreateBody(&player1->bodydef);
    player1->body->CreateFixture(&player1->fixturedef);

    player_array.push_back(player1);
}
