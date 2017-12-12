#pragma once

#include "stdafx.h"

class ModeSelect {
public:
    ModeSelect();

    void Update();
    void Draw(sf::RenderWindow& window);

    sf::Font arial;
    sf::Font banksia;

    int selection = 900;
    std::vector<sf::Text> player_text_array; // Texts in player-mode selection

private:
    sf::Text player_select;
    sf::Text two_players;
    sf::Text three_players;
    sf::Text four_players;
};
