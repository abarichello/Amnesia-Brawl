#include "powerup.h"

PowerUp::PowerUp(b2World& world) {
    rect.setSize(sf::Vector2f(25.f, 25.f));
    rect.setFillColor(sf::Color(200, 150, 0));

    /* Effects:
    1.  Explosion
    2.  Speed
    3.  Rage mode (invincibility + death on touch)
    4.  Float mode
    */
    // effect = GenerateRandom(3);
    effect = 4;

    // const float density = 1;
    // bodydef.type = b2_dynamicBody;
    // body = world.CreateBody(&bodydef);

    // shape.SetAsBox(rect.getLocalBounds().width / 2 / SCALE, rect.getLocalBounds().height / SCALE);
    // body->CreateFixture(&shape, density);
}

// PowerUp::~PowerUp() {
//     body->GetWorld()->DestroyBody(body);
// }

void PowerUp::Speed(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->max_speed = 30.f;
    iter->second->jump_impulse = 20.f;
}

void PowerUp::Floaty(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->body->SetGravityScale(0.2f);
}

void PowerUp::ResetPowerupEffects(std::map<std::size_t, Player*>::const_iterator& iter) {
    iter->second->max_speed = 10.f;
    iter->second->jump_impulse = 15.f;
    iter->second->body->SetGravityScale(1.0f);
}

void PowerUp::Update(sf::Time elapsed_time) {
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());
}

void PowerUp::Draw(sf::RenderWindow& window) {
    window.draw(rect);
}
