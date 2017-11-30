#pragma once

#include "stdafx.h"
#include "player.h"

class HUD {
public:
    HUD();

    void Update(Player* p1, Player* p2);
    void Draw(sf::RenderWindow& window);

private:
    sf::Font hud_font;
    sf::Text p1_score;
    sf::Text p2_score;
};
