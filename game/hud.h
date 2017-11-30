#pragma once

#include "stdafx.h"
#include "player.h"

class HUD {
public:
    HUD(std::size_t number_of_players);
    void Update();
    void Draw(sf::RenderWindow& window);
    std::vector<sf::Text> score_array;

    std::size_t number_of_players;
    std::size_t p1_score;
    std::size_t p2_score;
    std::size_t p3_score;
    std::size_t p4_score;

private:
    sf::Font hud_font;
    sf::Text score;

};
