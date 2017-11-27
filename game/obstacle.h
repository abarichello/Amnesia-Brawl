#pragma once

#include "stdafx.h"
#include "entity.h"

class Obstacle : public Entity {
public:
    Obstacle();

    bool is_ground;
};
