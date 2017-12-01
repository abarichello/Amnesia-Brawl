#include "powerup.h"

PowerUp::PowerUp() {
    rect.setSize(sf::Vector2f(25.f, 25.f));
    rect.setFillColor(sf::Color(200, 150, 0));

    /* Effects:
    1.  Explosion
    2.  Speed
    3.  Rage mode (invincibility + death on touch)
    */
    // effect = GenerateRandom(3);
    effect = 2;
}

void PowerUp::Draw(sf::RenderWindow& window) {
    window.draw(rect);
}
