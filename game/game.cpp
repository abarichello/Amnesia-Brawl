#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA BRAWL", sf::Style::Fullscreen),
    game_view(sf::FloatRect(0, 0, GAME_WIDTH, GAME_HEIGHT)),
    gravity(0.f, 18.f),
    world(gravity) {
    LoadResources();
}

void Game::Start() {
    window.setFramerateLimit(80);
    // window.setVerticalSyncEnabled(true);

    float countdown = ROUND_TIME; // Round duration
    Timer powerup_clock;

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (game_state == GameState::STATE_TITLE) {

                if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1)) {
                    window.close();
                } else if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 0)) {
                    levelselect = new class LevelSelect();
                    mode_select = new class ModeSelect();
                    game_state = GameState::STATE_MODE_SELECT;
                }

            } else if (game_state == GameState::STATE_MODE_SELECT) {

                if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1)) {
                    title_screen = new class TitleScreen();
                    game_state = GameState::STATE_TITLE;
                    delete levelselect;
                    delete mode_select;
                }
                // Down D-pad
                if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) || sf::Joystick::isButtonPressed(0, 13)) {
                    mode_select->selection += 1;
                }
                // Up D-Pad
                if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up) || sf::Joystick::isButtonPressed(0, 14)) {
                    mode_select->selection -= 1;
                }
                if (((event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)) || sf::Joystick::isButtonPressed(0, 0))) {
                    game_state = GameState::STATE_LEVEL_SELECT;
                }

            } else if (game_state == GameState::STATE_LEVEL_SELECT) {

                if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1)) {
                    game_state = GameState::STATE_MODE_SELECT;
                }
                if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) || sf::Joystick::isButtonPressed(0, 11) || sf::Joystick::isButtonPressed(0, 13)) {
                    levelselect->selection += 1;
                }
                if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up) || sf::Joystick::isButtonPressed(0, 12) || sf::Joystick::isButtonPressed(0, 14)) {
                    levelselect->selection -= 1;
                }
                if (((event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)) || sf::Joystick::isButtonPressed(0, 0))) {
                    global_clock.restart(); // Restart main clock
                    powerup_clock.Start(); // Start powerup clock
                    CreateRound(mode_select->selection % 3 + 2, levelselect->selection % 3 + 1, world); // Number of players / Map number / world
                    countdown = ROUND_TIME;
                    game_state = GameState::STATE_PLAY;
                }

            } else if (game_state == GameState::STATE_PLAY) {

                // Start - pauses
                if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7)) {
                    game_state = GameState::STATE_PAUSE;
                }

            } else if (game_state == GameState::STATE_PAUSE) {

                // Start - unpauses
                if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7)) {
                    powerup_clock.Start(); // Unpause powerup clock
                    game_state = GameState::STATE_PLAY;
                // Select/back Quits
                } else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) || sf::Joystick::isButtonPressed(0, 6)) {
                    game_state = GameState::STATE_LEVEL_SELECT;
                    EndRound();
                }

            }
        }

        switch (game_state) {
            case STATE_TITLE:
                TitleScreen();
                break;
            case STATE_MODE_SELECT:
                ModeSelect();
                break;
            case STATE_LEVEL_SELECT:
                LevelSelect();
                break;
            case STATE_PLAY: {
                sf::Time elapsed_time = global_clock.restart();
                GameLoop(elapsed_time, countdown, powerup_clock);
                break;
            }
            case STATE_PAUSE:
                global_clock.restart(); // Restart main clock
                sf::Time pause_time = sf::seconds(0); // Freeze time
                powerup_clock.Pause();
                GameLoop(pause_time, countdown, powerup_clock);
                break;
        }
    }
}

void Game::GameLoop(sf::Time elapsed_time, float& countdown, Timer& powerup_clock) {
    // GAME LOOP
    // Delta time between frames
    countdown -= elapsed_time.asSeconds();

    world.Step(elapsed_time.asSeconds() * 1.4f, 10, 10);
    window.clear();

    // Draw map
    map->Update();
    map->Draw(window);

    // Update players
    std::map<std::size_t, std::shared_ptr<Player>>::const_iterator itr = _game_object_manager._game_objects.begin();
    while (itr != _game_object_manager._game_objects.end()) {
        itr->second->rect.setPosition(SCALE * itr->second->body->GetPosition().x, SCALE * itr->second->body->GetPosition().y);
        itr->second->rect.setRotation(itr->second->body->GetAngle() * 180 / b2_pi);
        itr->second->Update(elapsed_time, map->obstacle_array);
        itr->second->Draw(window);
        itr++;
    }

    // Update springs
    itr = _game_object_manager._game_objects.begin();
    while (itr != _game_object_manager._game_objects.end()) {
        for (auto spring : map->spring_array) {
            spring.Update(itr, elapsed_time);
            spring.Draw(window);
        }
        ++itr;
    }

    // Update teleport pads
    itr = _game_object_manager._game_objects.begin();
    while (itr != _game_object_manager._game_objects.end()) {
        for (auto tp : map->tpad_array) {
            tp.Update(itr, elapsed_time);
            tp.Draw(window);
        }
        ++itr;
    }

    // Player-Player collision
    std::map<std::size_t, std::shared_ptr<Player>>::const_iterator iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end() && countdown > 0) { // Stop collisions after game ends
        std::map<std::size_t, std::shared_ptr<Player>>::const_iterator iter2 = _game_object_manager._game_objects.begin();
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
    if (powerup_clock.GetElapsedSeconds() > 7.f && powerup_array.size() == 0 && countdown > 0) {
        powerup_clock.Reset();
        PowerUp powerup;
        powerup.rect.setPosition(50 + GenerateRandom(GAME_WIDTH - 70), 50 + GenerateRandom(GAME_HEIGHT - 70));
        powerup_array.push_back(powerup);
        ResetPowerups();
    } else if (powerup_array.size() == 1 && powerup_clock.GetElapsedSeconds() > 10.f) { // Clean not picked up powerups
        powerup_clock.Reset();
        powerup_array.pop_back();
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
    while (iter != _game_object_manager._game_objects.end() && countdown > 0) {
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
                        powerup.Rage(iter);
                        iter->second->powered_up = true;
                        break;
                    case 4:
                        powerup.Floaty(iter);
                        iter->second->powered_up = true;
                        break;
                    case 5:
                        powerup.Immunity(iter);
                        iter->second->powered_up = true;
                        break;
                }
                iter2->alive = false;
                powerup_clock.Reset();
            }
            ++iter2;
        }
        ++iter;
    }

    // HUD Updates
    iter = _game_object_manager._game_objects.begin();
    hud->Update(iter, countdown);
    hud->Draw(window);

    // Winner check
    if (countdown < 0) {
        WinnerCheck();
        if (map->win_screen_countdown > 0) {
            map->win_screen_countdown -= elapsed_time.asSeconds();
            if (map->win_screen_countdown <= 0) {
                window.setView(game_view);
                EndRound();
                game_state = GameState::STATE_TITLE;
            }
        }
    }

    if (game_state == GameState::STATE_PAUSE) {
        window.draw(control_shape); // Pause menu
    }

    window.display();
}

void Game::TitleScreen() {
    title_screen->Draw(window);
}

void Game::ModeSelect() {
    mode_select->Update();
    mode_select->Draw(window);
}

void Game::LevelSelect() {
    levelselect->Update();
    levelselect->Draw(window);
}

void Game::CreateRound(std::size_t players_num, std::size_t level_number, b2World& world) {
    LoadPlayers(players_num);
    hud = new HUD(players_num);
    map = new Map(level_number, world);
}

void Game::LoadPlayers(std::size_t number_of_players) {
    // Create them again
    player1 = std::make_shared<Player>();
    player2 = std::make_shared<Player>();
    SpawnPlayer(1, player1, amnesia_blue, sf::Keyboard::Key::W, sf::Keyboard::Key::A, sf::Keyboard::Key::D);
    SpawnPlayer(2, player2, amnesia_red, sf::Keyboard::Key::Up, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right);
    if (number_of_players > 2 && number_of_players < 5) {
        player3 = std::make_shared<Player>();
        SpawnPlayer(3, player3, amnesia_dark_blue, sf::Keyboard::Key::T, sf::Keyboard::Key::F, sf::Keyboard::Key::H);
        if (number_of_players == 4) {
            player4 = std::make_shared<Player>();
            SpawnPlayer(4, player4, amnesia_dark_red, sf::Keyboard::Key::I, sf::Keyboard::Key::J, sf::Keyboard::Key::L);
        }
    }
}

void Game::SpawnPlayer(std::size_t number, std::shared_ptr<Player> player, sf::Color color, sf::Keyboard::Key jump, sf::Keyboard::Key left, sf::Keyboard::Key right) {
    // Player controls configurations
    player->number = number;
    player->jump = jump;
    player->left = left;
    player->right = right;
    player->sprite.setColor(color);
    CreatePlayer(world, player, spawn_locations[number-1].x, spawn_locations[number-1].y);
}

// Load game sprites
void Game::LoadResources() {
    if (!control_texture.loadFromFile(CONTROLS_TEXTURE)) {
        std::cout << "Error loading controls texture" << "\n";
    }

    // Default spawn locations
    spawn_locations.push_back(b2Vec2(GAME_WIDTH/3, GAME_HEIGHT/3));
    spawn_locations.push_back(b2Vec2(GAME_WIDTH - GAME_WIDTH/3 + 25, GAME_HEIGHT/3));
    spawn_locations.push_back(b2Vec2(GAME_WIDTH/6, GAME_HEIGHT - GAME_HEIGHT/3));
    spawn_locations.push_back(b2Vec2(GAME_WIDTH - GAME_WIDTH/6, GAME_HEIGHT - GAME_HEIGHT/3));

    title_screen = new class TitleScreen();

    amnesia_blue = sf::Color(14, 77, 203);
    amnesia_red = sf::Color(227, 12, 18);
    amnesia_dark_blue = sf::Color(29, 12, 137);
    amnesia_dark_red = sf::Color(158, 0, 0);

    control_shape.setTexture(&control_texture);
    control_shape.setSize(sf::Vector2f(control_shape.getTexture()->getSize().x, control_shape.getTexture()->getSize().y));
    control_shape.setPosition(GAME_WIDTH - control_shape.getLocalBounds().width, GAME_HEIGHT/2 - control_shape.getLocalBounds().height/2);

    background_music.openFromFile(BACKGROUND_SONG);
    background_music.play();
}

void Game::CreatePlayer(b2World& world, std::shared_ptr<Player> player, int x, int y) {
    // Creating physics body
    const float density = 1;
    player->bodydef.type = b2_dynamicBody;
    player->bodydef.position.Set(x / SCALE, y / SCALE);
    player->body = world.CreateBody(&player->bodydef);
    player->fixturedef.friction = 1.f;

    // Width and height subtracted by one, so the rect can intersect with the ground
    player->shape.SetAsBox(23 / SCALE, 23 / SCALE); // Main player body
    player->body->CreateFixture(&player->shape, density);

    _game_object_manager.Add(player->number, player);
}

void Game::WinnerCheck() {
    // Iterate to get the highest score
    auto iter = _game_object_manager._game_objects.begin();
    auto winner = iter->second;
    auto winner_score = iter->second->score;
    while (iter != _game_object_manager._game_objects.end()) {
        if (iter->second->score > winner_score) {
            winner = iter->second;
            winner_score = iter->second->score;
        }
        ++iter;
    }

    // Update winner text
    hud->winner_text.setOrigin(hud->winner_text.getLocalBounds().width / 2, hud->winner_text.getLocalBounds().height / 2);
    hud->winner_text.setPosition(winner->rect.getPosition().x, winner->rect.getPosition().y - 100);
    hud->winner_text.setString("Player  " + std::to_string(winner->number) + "  wins!");
    window.draw(hud->winner_text);

    // Center view to winner
    sf::View winner_view;
    winner_view.setSize(GAME_WIDTH / 2, GAME_HEIGHT / 2);

    float halfviewX = GAME_WIDTH / 2 * 0.5f;
    float halfviewY = GAME_HEIGHT / 2 * 0.5f;

    // Keep the camera inbounds
    // X Axis
    if (winner->rect.getPosition().x < halfviewX) {
        winner->rect.setPosition(halfviewX, winner->rect.getPosition().y);
    } else if (winner->rect.getPosition().x > GAME_WIDTH - halfviewX) {
        winner->rect.setPosition(GAME_WIDTH - halfviewX, winner->rect.getPosition().y);
    }

    // Y Axis
    if (winner->rect.getPosition().y < halfviewY) {
        winner->rect.setPosition(winner->rect.getPosition().x, halfviewY);
    } else if (winner->rect.getPosition().y > GAME_HEIGHT - halfviewY) {
        winner->rect.setPosition(winner->rect.getPosition().x, GAME_HEIGHT - halfviewY);
    }

    winner_view.setCenter(winner->rect.getPosition());
    window.setView(winner_view);
}

void Game::ResetPowerups() {
    std::map<std::size_t, std::shared_ptr<Player>>::const_iterator iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end()) {
        if (iter->second->powered_up) {
            PowerUp::ResetPowerupEffects(iter);
            iter->second->powered_up = false;
        }
        ++iter;
    }
}

void Game::EndRound() {
    // Function called while exiting a round
    delete map;
    delete hud;
    auto i = 1;

    // Delete players physics bodies
    std::map<std::size_t, std::shared_ptr<Player>>::const_iterator iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end()) {
        iter->second->body->GetWorld()->DestroyBody(iter->second->body);
        _game_object_manager.Remove(i);
        ++i;
        ++iter;
    }
    window.setView(game_view);

    // Delete powerups
    for (auto pwup : powerup_array) {
        powerup_array.pop_back();
    }
}

GameObjectManager Game::_game_object_manager;
