#include "obstacle.h"

Obstacle::Obstacle() {
    rect.setSize(sf::Vector2f(50.f, 50.f));
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::White);
}