#include "stdafx.h"
#include "player.h"

Player::Player() {
    rect.setSize(sf::Vector2f(40.f, 40.f));
    rect.setPosition(GAME_WIDTH/2, GAME_HEIGHT/2);
    rect.setFillColor(sf::Color(255, 100, 100));
    rect.setOrigin(16.f, 16.f);
}
