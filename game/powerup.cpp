#include "powerup.h"

PowerUp::PowerUp() {
    rect.setSize(sf::Vector2f(50.f, 50.f));
    rect.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);

    /* Effects:
    1.  Invisibility - Tequila
    2.  Speed - Açaí
    3.  RAGE MODE (Immunity + death on touch) - Vodka
    4.  Float mode - Absinto
    5.  Immunity - Beer
    */

    effect = GenerateRandom(4);
    // effect = 4; // DEBUG
    switch (effect) {
        case 1:
            rect.setFillColor(sf::Color(100, 100, 100)); // Invisibility - Tequila
            Load(TEQUILA_TEXTURE);
            break;
        case 2:
            rect.setFillColor(sf::Color(0, 100, 200)); // Speed - Açaí
            Load(ACAI_TEXTURE);
            break;
        case 3:
            rect.setFillColor(sf::Color(155, 10, 10)); // RAGE MODE - Vodka
            Load(VODKA_TEXTURE);
            break;
        case 4:
            rect.setFillColor(sf::Color(123, 184, 255)); // Floaty - Absinto
            Load(ABSINTO_TEXTURE);
            break;
        case 5:
            rect.setFillColor(sf::Color(255, 239, 0)); // Immunity - Beer
            Load(BEER_TEXTURE);
            break;
    }

    sprite.setOrigin(rect.getLocalBounds().width / 2, rect.getLocalBounds().height / 2);
    sprite.setTextureRect(sf::IntRect(0, 0, rect.getLocalBounds().width, rect.getLocalBounds().height));
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
    iter->second->rectB.setSize(sf::Vector2f(64, 110)); // Some magic numbers
    iter->second->rectB.setOrigin(sf::Vector2f(32, 80)); // set rectB position to the same of rect
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
    rect.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y + std::sin(countdown * 3) * floating_effect)); // Floating effect
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());
}

void PowerUp::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
