#pragma once

#include "stdafx.h"
#include "entity.h"
#include "player.h"

class TeleportPad : public Entity {
public:
    TeleportPad();
    void SetDestination(b2Vec2 destination);

    void Update(std::map<std::size_t, Player*>::const_iterator &iter, sf::Time elapsed_time);
    void Draw(sf::RenderWindow& window);

    sf::Texture tpad_texture;
    sf::Sprite tpad_sprite;

private:
    b2Vec2 destination;
};
