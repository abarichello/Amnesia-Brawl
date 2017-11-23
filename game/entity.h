#pragma once

#include "stdafx.h"
#include "visible_game_object.h"

class Entity : public VisibleGameObject {
public:
    float hp = 0;
    bool alive = true;
    int angle = 0;
};
