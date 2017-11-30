#include "hud.h"

HUD::HUD(std::size_t number_of_players) {
    if (!hud_font.loadFromFile(HUD_FONT)) {
        std::cout << "Error loading font!" << "\n";
    }

    this->number_of_players = number_of_players;

    for (auto i = 0; i < number_of_players; ++i) {
        score.setFont(hud_font);
        score.setOutlineColor(sf::Color::White);
        score.setOutlineThickness(1);
        score.setCharacterSize(30);
        score.setString("0");
        score_array.push_back(score);
    }

    score_array[0].setFillColor(sf::Color(24, 165, 235));
    score_array[1].setFillColor(sf::Color(227, 46, 18));
    score_array[0].setPosition(score.getLocalBounds().width/2, GAME_HEIGHT - score.getLocalBounds().height);
    score_array[1].setPosition(GAME_WIDTH - score.getLocalBounds().width * 5/2, GAME_HEIGHT - score.getLocalBounds().height);
    if (number_of_players == 4) {
        score_array[2].setFillColor(sf::Color(0, 105, 145));
        score_array[3].setFillColor(sf::Color(157, 6, 0));
        score_array[2].setPosition(score.getLocalBounds().width/2, score.getLocalBounds().height/2);
        score_array[3].setPosition(GAME_WIDTH - score.getLocalBounds().width * 5/2, score.getLocalBounds().height/2);
    }
}

void HUD::Update() {
    score_array[0].setString(std::to_string(p1_score));
    score_array[1].setString(std::to_string(p2_score));
    if (number_of_players == 4) {
        score_array[2].setString(std::to_string(p3_score));
        score_array[3].setString(std::to_string(p4_score));
    }
}

void HUD::Draw(sf::RenderWindow& window) {
    for (auto score : score_array) {
        window.draw(score);
    }
}
