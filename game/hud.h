#pragma once

#include "stdafx.h"
#include "player.h"

class HUD {
public:
    HUD(std::size_t number_of_players);
    void Update(std::map<std::size_t, std::shared_ptr<Player>>::const_iterator &iter, float coundown);
    void Draw(sf::RenderWindow& window);
    std::vector<sf::Text> score_array;

    std::size_t number_of_players;
    
    sf::Text score;
    sf::Text match_clock;
    sf::Text winner_text;

private:
    sf::Font hud_font;
    sf::Font bonbon_font;
};
