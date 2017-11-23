#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA GAME") {
    Start();
}

void Game::Start() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            window.close();
        }

        sf::Time elapsed_time = global_clock.restart();

        GameLoop(window, elapsed_time);
    }
}

void Game::GameLoop(sf::RenderWindow& window, sf::Time elapsed_time) {
    window.clear();
    window.draw();
    window.display();
}
