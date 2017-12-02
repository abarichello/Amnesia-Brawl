#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA BRAWL"),
    gravity(0.f, 18.f),
    world(gravity),
    hud(4) {

    amnesia_blue = sf::Color(14, 77, 203);
    amnesia_red = sf::Color(227, 12, 18);
    amnesia_dark_blue = sf::Color(29, 12, 137);
    amnesia_dark_red = sf::Color(158, 0, 0);

    map = new Map(1, world, obstacle_array);
    LoadResources();
    LoadPlayers(4);
}

void Game::Start() {
    window.setFramerateLimit(70);
    float countdown = 180.f;
    powerup_clock.restart();

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
        countdown -= elapsed_time.asSeconds();

        world.Step(1.f/60.f, 10, 10);
        window.clear();

        // Update players
        std::map<std::size_t, Player*>::const_iterator itr = _game_object_manager._game_objects.begin();
        while (itr != _game_object_manager._game_objects.end()) {
            itr->second->rect.setPosition(SCALE * itr->second->body->GetPosition().x, SCALE * itr->second->body->GetPosition().y);
            itr->second->rect.setRotation(itr->second->body->GetAngle() * 180 / b2_pi);
            itr->second->Update(elapsed_time, obstacle_array);
            itr->second->Draw(window);
            itr++;
        }

        // Update walls
        for (auto wall : obstacle_array) {
            wall.rect.setPosition(SCALE * wall.body->GetPosition().x, SCALE * wall.body->GetPosition().y);
            wall.rect.setRotation(wall.body->GetAngle() * 180 / b2_pi);
            window.draw(wall.rect);
        }

        // Player-Player collision
        std::map<std::size_t, Player*>::const_iterator iter = _game_object_manager._game_objects.begin();
        while (iter != _game_object_manager._game_objects.end()) {
            std::map<std::size_t, Player*>::const_iterator iter2 = _game_object_manager._game_objects.begin();
            while (iter2 != _game_object_manager._game_objects.end()) {
                if (iter->second->rectB.getGlobalBounds().intersects(iter2->second->rectA.getGlobalBounds())) {
                    iter->second->score += 1;
                    iter->second->jumps_remaining = 1;
                    iter2->second->Respawn();
                }
                ++iter2;
            }
            ++iter;
        }

        // Powerup generation
        if (powerup_clock.getElapsedTime().asSeconds() > 4.f && powerup_array.size() < 1) {
            powerup_clock.restart();
            PowerUp powerup;
            powerup.rect.setPosition(50 + GenerateRandom(GAME_WIDTH - 70), 50 + GenerateRandom(GAME_HEIGHT - 70));
            powerup_array.push_back(powerup);
            ResetPowerups();
        } else if (powerup_array.size() == 1) {
            powerup_clock.restart();
        }

        // Powerup drawing
        for (auto powerup : powerup_array) {
            powerup.Update(countdown);
            powerup.Draw(window);
            if (!powerup.alive) {
                powerup_array.pop_back();
            }
        }

        // Powerup collision
        iter = _game_object_manager._game_objects.begin();
        while (iter != _game_object_manager._game_objects.end()) {
            std::vector<PowerUp>::iterator iter2 = powerup_array.begin();
            while (iter2 != powerup_array.end()) {
                if (iter->second->rect.getGlobalBounds().intersects(iter2->rect.getGlobalBounds())) {
                    auto powerup = powerup_array[0];
                    switch (iter2->effect) {
                        case 1:
                            powerup.Invisibility(iter);
                            iter->second->powered_up = true;
                            break;
                        case 2:
                            powerup.Speed(iter);
                            iter->second->powered_up = true;
                            break;
                        case 3:
                            powerup.Immunity(iter);
                            iter->second->powered_up = true;
                            break;
                        case 4:
                            powerup.Floaty(iter);
                            iter->second->powered_up = true;
                            break;
                    }
                    iter2->alive = false;
                }
                ++iter2;
            }
            ++iter;
        }

        // HUD Updates
        iter = _game_object_manager._game_objects.begin();
        hud.Update(iter, countdown);
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
    CreatePlayer(world, player, GenerateRandom(GAME_WIDTH), GenerateRandom(GAME_HEIGHT));
}

// Load game sprites
void Game::LoadResources() {
    // TODO
}

void Game::CreatePlayer(b2World& world, Player* player, int x, int y) {
    const float density = 1;
    player->bodydef.type = b2_dynamicBody;
    player->bodydef.position.Set(x / SCALE, y / SCALE);
    player->body = world.CreateBody(&player->bodydef);

    // Width and height subtracted by one, so the rect can intersect with the ground
    player->shape.SetAsBox(23 / SCALE, 23 / SCALE); // Main player body
    player->body->CreateFixture(&player->shape, density);

    _game_object_manager.Add(player->number, player);
}

void Game::ResetPowerups() {
    std::map<std::size_t, Player*>::const_iterator iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end()) {
        if (iter->second->powered_up) {
            PowerUp::ResetPowerupEffects(iter);
            iter->second->powered_up = false;
        }
        ++iter;
    }
}

GameObjectManager Game::_game_object_manager;
