#include "powerup.h"

PowerUp::PowerUp() {
    rect.setSize(sf::Vector2f(25.f, 25.f));
    rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);

    /* Effects:
    1.  Invisibility
    2.  Speed
    3.  Immunity
    4.  Float mode
    5.  Stun other players
    */

    effect = GenerateRandom(4);
    // effect = 3;
    switch (effect) {
        case 1:
            rect.setFillColor(sf::Color(100, 100, 100)); // Invisibility
            break;
        case 2:
            rect.setFillColor(sf::Color(0, 100, 200)); // Speed
            break;
        case 3:
            rect.setFillColor(sf::Color(255, 196, 0)); // Immunity
            break;
        case 4:
            rect.setFillColor(sf::Color(123, 184, 255)); // Floaty
            break;
    }   
}

void PowerUp::Invisibility(std::map<std::size_t, Player*>::const_iterator& iter) {
    auto color = iter->second->rect.getFillColor();
    color.a = 55;
    iter->second->rect.setFillColor(color);
}

void PowerUp::Speed(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->max_speed = 30.f;
    iter->second->jump_impulse = 20.f;
}

void PowerUp::Immunity(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->rectA.setSize(sf::Vector2f(0, 0));
}

void PowerUp::Floaty(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->body->SetGravityScale(0.2f);
}

void PowerUp::ResetPowerupEffects(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->max_speed = 10.f;
    iter->second->jump_impulse = 15.f;
    iter->second->body->SetGravityScale(1.0f);
    auto color = iter->second->rect.getFillColor();
    color.a = 255;
    iter->second->rect.setFillColor(color);
    iter->second->rectA.setSize(sf::Vector2f(30, 4));
}

void PowerUp::Update(float countdown) {
    rect.setRotation(countdown * rotation);

    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());
}

void PowerUp::Draw(sf::RenderWindow& window) {
    window.draw(rect);
}
