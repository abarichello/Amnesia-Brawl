#include "hud.h"

HUD::HUD() {
    if (!hud_font.loadFromFile(HUD_FONT)) {
        std::cout << "Error loading font!" << "\n";
    }

    p1_score.setFont(hud_font);
    p2_score.setFont(hud_font);

    p1_score.setFillColor(sf::Color(24, 165, 235));
    p2_score.setFillColor(sf::Color(227, 46, 18));
    p1_score.setOutlineColor(sf::Color::White);
    p2_score.setOutlineColor(sf::Color::White);
    p1_score.setOutlineThickness(1);
    p2_score.setOutlineThickness(1);

    p1_score.setCharacterSize(30);
    p2_score.setCharacterSize(30);
    p1_score.setString("0");
    p2_score.setString("0");

    p1_score.setPosition(p1_score.getLocalBounds().width/2, GAME_HEIGHT - p1_score.getLocalBounds().height);
    p2_score.setPosition(GAME_WIDTH - p2_score.getLocalBounds().width, GAME_HEIGHT - p2_score.getLocalBounds().height);
}

void HUD::Update(Player* p1, Player* p2) {
    p1_score.setString(std::to_string(p1->score));
    p2_score.setString(std::to_string(p2->score));
}

void HUD::Draw(sf::RenderWindow& window) {
    window.draw(p1_score);
    window.draw(p2_score);
}
