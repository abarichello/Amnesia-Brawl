#pragma once

#include "stdafx.h"
#include "player.h"

class HUD {
public:
    HUD(std::size_t number_of_players);
    void Update(std::map<std::size_t, Player*>::const_iterator &iter);
    void Draw(sf::RenderWindow& window);
    std::vector<sf::Text> score_array;

    std::size_t number_of_players;

private:
    sf::Font hud_font;
    sf::Text score;
};
