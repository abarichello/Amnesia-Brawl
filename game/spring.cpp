#include "spring.h"

Spring::Spring() {
    rect.setSize(sf::Vector2f(40.f, 20.f));
    rect.setFillColor(sf::Color(0, 255, 255));
    rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);
    sprite.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);
    sprite.setTextureRect(sf::IntRect(0, 0, rect.getLocalBounds().width, rect.getLocalBounds().height));
}

void Spring::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Spring::Update(std::map<std::size_t, std::shared_ptr<Player>>::const_iterator& iter, sf::Time elapsed_time) {
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());

    if (iter->second->rect.getGlobalBounds().intersects(rect.getGlobalBounds())) {
        iter->second->body->ApplyForceToCenter(b2Vec2(0, -spring_propulsion * elapsed_time.asSeconds()), true);
        return true;
    }
    return false;
}
