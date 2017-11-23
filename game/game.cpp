#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA GAME") {
    LoadSprites();
    Start();
}

void Game::Start() {
    window.setFramerateLimit(70);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            sf::Time elapsed_time = global_clock.restart();
            GameLoop(window, elapsed_time);
        }
    }
}

void Game::GameLoop(sf::RenderWindow& window, sf::Time elapsed_time) {
    window.clear();
    
    for (auto player : player_array) {
        window.draw(player->rect);
    }

    for (auto obstacle : obstacle_array) {
        window.draw(obstacle.rect);
    }

    window.display();
}

void Game::LoadSprites() {
    player1 = new Player();
    player_array.push_back(player1);

    Obstacle obstacle;
    obstacle.rect.setSize(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT/17));
    obstacle.rect.setPosition(0, GAME_HEIGHT - obstacle.rect.getGlobalBounds().height);
    obstacle_array.push_back(obstacle);
}
