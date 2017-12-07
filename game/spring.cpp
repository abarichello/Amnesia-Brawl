#include "spring.h"

Spring::Spring() {
    rect.setSize(sf::Vector2f(40.f, 20.f));
    rect.setFillColor(sf::Color(0, 255, 255));
    rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);
}

void Spring::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Spring::Update(std::map<std::size_t, Player*>::const_iterator &iter, sf::Time elapsed_time) {
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());

    if (iter->second->rect.getGlobalBounds().intersects(rect.getGlobalBounds())) {
        iter->second->body->ApplyForceToCenter(b2Vec2(0, -spring_propulsion * elapsed_time.asSeconds()), true);
    }
}
