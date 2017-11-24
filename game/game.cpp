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
    CreatePlayers(world, 100.f, 100.f);

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

        sf::Time elapsed_time = global_clock.restart();
        GameLoop(window, elapsed_time);
    }
}

void Game::GameLoop(sf::RenderWindow& window, sf::Time elapsed_time) {
    world.Step(elapsed_time.asSeconds(), 6, 3);
    window.clear();
    
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

    for (auto player : player_array) {
        window.draw(player->rect);
    }

    // for (auto obstacle : obstacle_array) {
    //     window.draw(obstacle.rect);
    // }

    window.display();
}

void Game::LoadResources() {
    
}


void Game::CreateGround(b2World& world) {
    ground.rect.setSize(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT/17));
    ground.rect.setPosition(0, GAME_HEIGHT - ground.rect.getLocalBounds().height);    
    
    ground.bodydef.position = b2Vec2(ground.rect.getPosition().x/SCALE, ground.rect.getPosition().y/SCALE);
    ground.bodydef.type = b2_staticBody;
    ground.shape.SetAsBox((ground.rect.getLocalBounds().width)/SCALE, (ground.rect.getLocalBounds().height)/SCALE);
    ground.fixturedef.density = 0.f;
    ground.fixturedef.shape = &ground.shape;

    ground.body = world.CreateBody(&ground.bodydef);
    ground.body->CreateFixture(&ground.fixturedef);

    obstacle_array.push_back(ground);
}

void Game::CreatePlayers(b2World& world, int x, int y) {
    player1 = new Player();
    player1->bodydef.position = b2Vec2(x/30.f, y/30.f);
    player1->bodydef.type = b2_dynamicBody;    
    player1->shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
    player1->fixturedef.density = 1.f;
    player1->fixturedef.shape = &player1->shape;

    player1->body = world.CreateBody(&player1->bodydef);
    player1->body->CreateFixture(&player1->fixturedef);

    player_array.push_back(player1);
}
