#include "stdafx.h"
#include "player.h"

Player::Player() {
    rect.setSize(sf::Vector2f(50.f, 50.f));
    rect.setPosition(GAME_WIDTH/2, GAME_HEIGHT/2);
    rect.setFillColor(sf::Color(255, 100, 100));
    rect.setOrigin(rect.getSize().x/2, rect.getSize().y/2);
}

void Player::Draw(sf::RenderWindow& window) {
    window.draw(rect);
}

void Player::Update(sf::Time elapsed_time, std::vector<Obstacle> &obstacle_array) {
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
                body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x , -jump_impulse));
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
 
    // Jump reset
    for (auto obstacle : obstacle_array) {
        if (rect.getGlobalBounds().intersects(obstacle.rect.getGlobalBounds()) && obstacle.is_ground) {
            jumps_remaining = 1;
        }
    }
}
