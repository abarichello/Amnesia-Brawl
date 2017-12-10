#pragma once

#include "stdafx.h"
#include "map.h"

class LevelSelect {
public:
    LevelSelect();

    void Update();
    void Draw(sf::RenderWindow& window);

    sf::Font arial;
    sf::Font banksia;

    std::size_t selection = 0;

private:
    sf::Text level_select;
    sf::Text level1;
    sf::Text level2;
    sf::Text level3;
};
