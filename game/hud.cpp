#include "hud.h"

HUD::HUD(std::size_t number_of_players) {
    if (!hud_font.loadFromFile(HUD_FONT)) {
        std::cout << "Error loading font!" << "\n";
    }

    this->number_of_players = number_of_players;

    for (auto i = 0u; i < number_of_players; ++i) {
        score.setFont(hud_font);
        score.setOutlineColor(sf::Color::White);
        score.setOutlineThickness(1);
        score.setCharacterSize(30);
        score.setString("0");
        score_array.push_back(score);
    }

    score_array[0].setFillColor(sf::Color(14, 77, 203));
    score_array[1].setFillColor(sf::Color(227, 12, 18));
    score_array[0].setPosition(score.getLocalBounds().width/2, GAME_HEIGHT - score.getLocalBounds().height);
    score_array[1].setPosition(GAME_WIDTH - score.getLocalBounds().width * 5/2, GAME_HEIGHT - score.getLocalBounds().height);
    if (number_of_players == 4) {
        score_array[2].setFillColor(sf::Color(29, 12, 137));
        score_array[3].setFillColor(sf::Color(158, 0, 0));
        score_array[2].setPosition(score.getLocalBounds().width/2, score.getLocalBounds().height/2);
        score_array[3].setPosition(GAME_WIDTH - score.getLocalBounds().width * 5/2, score.getLocalBounds().height/2);
    }
}

void HUD::Update(std::map<std::size_t, Player*>::const_iterator &iter) {
    for (auto i = 0u; i < number_of_players; ++i) {
        score_array[i].setString(std::to_string(iter->second->score));
        ++iter;
    }
}

void HUD::Draw(sf::RenderWindow& window) {
    for (auto score : score_array) {
        window.draw(score);
    }
}
