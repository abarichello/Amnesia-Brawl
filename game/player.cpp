#include "stdafx.h"
#include "player.h"

Player::Player() {
    rect.setSize(sf::Vector2f(50.f, 50.f));
    rect.setPosition(GAME_WIDTH/2, GAME_HEIGHT/2);
    rect.setFillColor(sf::Color(255, 100, 100));
    rect.setOrigin(rect.getSize().x/2, rect.getSize().y/2);
}

void Player::Update(sf::Time elapsed_time) {
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());

    if (alive) {
        float move_amount = movement_speed * elapsed_time.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            rect.move(-move_amount, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            rect.move(move_amount, 0);
        }
    }
}
