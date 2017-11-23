#include "stdafx.h"
#include "entity.h"

void Entity::Load(std::string filename) {
    if(!texture.loadFromFile(filename)) {
        filename = "";
    } else {
        filename = filename;
        sprite.setTexture(texture);
        loaded = true;
    }
}

void Entity::Draw(sf::RenderWindow& window) {
    if (loaded) {
        window.draw(sprite);
    }
}

void Entity::Update(float elapsed_time) {}

float Entity::GetWidth() const {
    return sprite.getLocalBounds().width;
}

float Entity::GetHeight() const {
    return sprite.getLocalBounds().height;
}

sf::Rect<float> Entity::GetBoundingRect() const {
    return sprite.getGlobalBounds();
}
