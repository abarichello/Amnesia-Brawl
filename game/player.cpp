#include "stdafx.h"
#include "player.h"

Player::Player() {
    rect.setSize(sf::Vector2f(40.f, 40.f));
    rect.setPosition(GAME_WIDTH/2, GAME_HEIGHT/2);
    rect.setFillColor(sf::Color(255, 100, 100));
    rect.setOrigin(rect.getSize().x/2, rect.getSize().y/2);
}

void Player::Update(sf::Time elapsed_time, Obstacle ground) {
    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());

    // body->SetAngularDamping(1);
    body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));

    if (alive) {
        float move_amount = movement_speed * elapsed_time.asSeconds();

        if (sf::Keyboard::isKeyPressed(left)) {
            // body->ApplyForce(b2Vec2(-move_amount, 0), body->GetWorldCenter(), false);
            body->SetLinearVelocity(b2Vec2(-move_amount, body->GetLinearVelocity().y));
        }

        if (sf::Keyboard::isKeyPressed(right)) {
            // body->ApplyForce(b2Vec2(move_amount, 0), body->GetWorldCenter(), false);
            body->SetLinearVelocity(b2Vec2(move_amount, body->GetLinearVelocity().y));
        }

        if (sf::Keyboard::isKeyPressed(jump)) {
            if (jumps_remaining > 0) {
                --jumps_remaining;
                b2Vec2 vel = body->GetLinearVelocity();
                vel.y = -jump_impulse * elapsed_time.asSeconds();
                body->SetLinearVelocity(vel);
            }
        }
    }
    
    // Max speed
    if (body->GetLinearVelocity().x > max_speed) {
        body->SetLinearVelocity(b2Vec2(max_speed, body->GetLinearVelocity().y));
    }
    if (body->GetLinearVelocity().x < -max_speed) {
        body->SetLinearVelocity(b2Vec2(-max_speed, body->GetLinearVelocity().y));
    }
    if (body->GetLinearVelocity().y > max_speed) {
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, max_speed));
    }
    if (body->GetLinearVelocity().y < -max_speed) {
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -max_speed));
    }

    // Jump reset
    if (rect.getGlobalBounds().intersects(ground.rect.getGlobalBounds())) {
        jumps_remaining = 1;
    }
}
