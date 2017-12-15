#include "teleport_pad.h"

TeleportPad::TeleportPad() {
    Load(ARROW_TEXTURE);
    rect.setSize(sf::Vector2f(20, 60));
    rect.setFillColor(sf::Color::Green);
    rect.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);

    dest_rec.setSize(sf::Vector2f(5, 5));
    dest_rec.setFillColor(sf::Color::Yellow);
}

void TeleportPad::SetDestination(b2Vec2 destination) {
    this->destination = destination;
}

void TeleportPad::Update(std::map<std::size_t, Player*>::const_iterator &iter, sf::Time elapsed_time) {
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());
    dest_rec.setPosition(destination.x * SCALE, destination.y * SCALE);

    if (iter->second->rect.getGlobalBounds().intersects(rect.getGlobalBounds())) {
        iter->second->body->SetTransform(destination, iter->second->body->GetAngle());
    }
}

void TeleportPad::Draw(sf::RenderWindow& window) {
    // window.draw(dest_rec); // DEBUG
    window.draw(rect);
}
