#pragma once

#include "stdafx.h"
#include "entity.h"

class Player : public Entity {
public:
    Player();

    float movement_speed = 60.f;
    
    bool alive = true;
};
