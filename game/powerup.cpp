#include "powerup.h"

PowerUp::PowerUp() {
    rect.setSize(sf::Vector2f(25.f, 25.f));
    rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);

    /* Effects:
    1.  Invisibility
    2.  Speed
    3.  RAGE MODE (Immunity + death on touch)
    4.  Float mode
    5.  Immunity
    */

    effect = GenerateRandom(4);
    // effect = 2;
    switch (effect) {
        case 1:
            rect.setFillColor(sf::Color(100, 100, 100)); // Invisibility
            break;
        case 2:
            rect.setFillColor(sf::Color(0, 100, 200)); // Speed
            break;
        case 3:
            rect.setFillColor(sf::Color(155, 10, 10)); // RAGE MODE
            break;
        case 4:
            rect.setFillColor(sf::Color(123, 184, 255)); // Floaty
            break;
        case 5:
            rect.setFillColor(sf::Color(255, 239, 0)); // Immunity
            break;
    }
}

void PowerUp::Invisibility(std::map<std::size_t, Player*>::const_iterator& iter) {
    auto color = iter->second->sprite.getColor();
    color.a = 95;
    iter->second->sprite.setColor(color);
}

void PowerUp::Speed(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->max_speed = 30.f;
    iter->second->jump_impulse = 20.f;
}

void PowerUp::Rage(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->rectA.setSize(sf::Vector2f(0, 0));
    iter->second->rectB.setSize(sf::Vector2f(58, 70));
    iter->second->rectB.setOrigin(sf::Vector2f(24, 48)); // set rectB position to the same of rect
}

void PowerUp::Floaty(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->body->SetGravityScale(0.2f);
}

void PowerUp::Immunity(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->rectA.setSize(sf::Vector2f(0, 0));
}

void PowerUp::ResetPowerupEffects(std::map<std::size_t, Player*>::const_iterator& iter) {
    // Reset speed
    iter->second->max_speed = 10.f;
    iter->second->jump_impulse = 15.f;

    // Reset floaty
    iter->second->body->SetGravityScale(1.0f);

    // Reset invisibility
    auto color = iter->second->sprite.getColor();
    color.a = 255;
    iter->second->sprite.setColor(color);

    // Reset RAGE and immunity mode
    iter->second->rectA.setSize(sf::Vector2f(HITBOX_X, HITBOX_Y));
    iter->second->rectB.setSize(sf::Vector2f(HITBOX_X, HITBOX_Y));
    iter->second->rectB.setOrigin(iter->second->rectB.getLocalBounds().width / 2, iter->second->rectB.getLocalBounds().height / 2);
}

void PowerUp::Update(float countdown) {
    rect.setRotation(countdown * rotation);

    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());
}

void PowerUp::Draw(sf::RenderWindow& window) {
    window.draw(rect);
}
