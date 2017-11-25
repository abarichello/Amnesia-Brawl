#pragma once

#include "stdafx.h"
#include "entity.h"

class Player : public Entity {
public:
    Player();
    void Update(sf::Time elapsed_time);

    float movement_speed = 100.f;
    float jump_impulse = 1000.f;
    bool alive = true;
};
