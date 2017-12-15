#include "teleport_pad.h"

TeleportPad::TeleportPad() {
    rect.setSize(sf::Vector2f(20, 60));
    rect.setFillColor(sf::Color::Green);
    rect.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);

    sprite.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);
}

void TeleportPad::SetDestination(b2Vec2 destination) {
    this->destination = destination;
}

void TeleportPad::Update(std::map<std::size_t, Player*>::const_iterator &iter, sf::Time elapsed_time) {
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());

    if (iter->second->rect.getGlobalBounds().intersects(rect.getGlobalBounds())) {
        iter->second->body->SetTransform(destination, iter->second->body->GetAngle());
    }
}

void TeleportPad::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
