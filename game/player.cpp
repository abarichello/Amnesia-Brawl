#include "stdafx.h"
#include "player.h"

Player::Player() {
    Load(PLAYER_TEXTURE);
    assert(loaded);

    rect.setSize(sf::Vector2f(48.f, 48.f));
    rect.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
    rect.setFillColor(sf::Color(255, 100, 100));
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);

    sprite.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);

    rectA.setSize(sf::Vector2f(HITBOX_X, HITBOX_Y));
    rectA.setOrigin(rectA.getLocalBounds().width / 2, rectA.getLocalBounds().height / 2);
    rectA.setFillColor(sf::Color::White);

    rectB.setSize(sf::Vector2f(HITBOX_X, HITBOX_Y));
    rectB.setOrigin(rectB.getLocalBounds().width / 2, rectB.getLocalBounds().height / 2);
    rectB.setFillColor(sf::Color::White);
}

Player::~Player() {
    body->GetWorld()->DestroyBody(body);
}

void Player::Draw(sf::RenderWindow& window) {
    // window.draw(rect);
    // window.draw(rectA);
    // window.draw(rectB); // DEBUG
    window.draw(sprite);
}

void Player::Respawn() {
    body->SetTransform(b2Vec2(GenerateRandom(GAME_WIDTH) / SCALE, GenerateRandom(GAME_HEIGHT) / SCALE), 0);
    body->SetLinearVelocity(b2Vec2(0, -5));
    jumps_remaining = 1;
}

void Player::Update(sf::Time elapsed_time, std::vector<Obstacle> &obstacle_array) {
    rectA.setPosition(rect.getPosition().x, rect.getPosition().y - rect.getLocalBounds().height/2 + 1); // Upper collision
    rectB.setPosition(rect.getPosition().x, rect.getPosition().y + rect.getLocalBounds().height/2 - 1); // Lower collision

    sprite.setPosition(rect.getPosition());
    sprite.setRotation(rect.getRotation());

    // Break the sprite x velocity
    body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));

    if (alive) {
        float move_amount = movement_speed * elapsed_time.asSeconds();

        if (sf::Keyboard::isKeyPressed(left)) {
            body->SetLinearVelocity(b2Vec2(-move_amount, body->GetLinearVelocity().y));
        }

        if (sf::Keyboard::isKeyPressed(right)) {
            body->SetLinearVelocity(b2Vec2(move_amount, body->GetLinearVelocity().y));
        }

        if (sf::Keyboard::isKeyPressed(jump)) {
            if (jumps_remaining > 0 && jump_clock.getElapsedTime().asSeconds() > 0.5f) {
                jump_clock.restart();
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
        if (rectB.getGlobalBounds().intersects(obstacle.rect.getGlobalBounds()) && obstacle.is_ground) {
            jumps_remaining = 1;
        }
    }

    // Out of bounds
    if (rect.getPosition().x > GAME_WIDTH + 200 || rect.getPosition().x < -500 ||
        rect.getPosition().y > GAME_HEIGHT + 200 || rect.getPosition().y < -500) {
        Respawn();
        score -= 1;
    }
}
