#include "level_select.h"

LevelSelect::LevelSelect() {
    if(!arial.loadFromFile(ARIEL_PIXEL_FONT)) {
        std::cout << "Error loading arial font!" << "\n";
    }
    if(!banksia.loadFromFile(BANKSIA_FONT)) {
        std::cout << "Error loading banksia font!" << "\n";
    }

    level_select.setFont(arial);
    level_select.setOutlineColor(sf::Color::White);
    level_select.setOutlineThickness(3);
    level_select.setFillColor(sf::Color(227, 12, 18));
    level_select.setCharacterSize(45);
    level_select.setString("Choose the map:");
    level_select.setOrigin(level_select.getLocalBounds().width / 2, level_select.getLocalBounds().height / 2);
    level_select.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 3 - GAME_HEIGHT / 6);

    level1.setFont(banksia);
    level1.setFillColor(sf::Color::Black);
    level1.setCharacterSize(35);
    level1.setString("Level 1 - Night Club");
    level1.setPosition(GAME_WIDTH / 14, GAME_HEIGHT / 3);

    level2.setFont(banksia);
    level2.setFillColor(sf::Color::Black);
    level2.setCharacterSize(35);
    level2.setString("Level 2 - ");
    level2.setPosition(GAME_WIDTH / 14, GAME_HEIGHT / 3 + level2.getLocalBounds().height);

    level3.setFont(banksia);
    level3.setFillColor(sf::Color::Black);
    level3.setCharacterSize(35);
    level3.setString("Level 3 - Tequila Desert");
    level3.setPosition(GAME_WIDTH / 14, GAME_HEIGHT / 3  + level2.getLocalBounds().height * 2);
    
    level_text_array.push_back(level1);
    level_text_array.push_back(level2);
    level_text_array.push_back(level3);

    // Common configs
    for (auto text : level_text_array) {
        text.setOutlineColor(sf::Color::White);
        text.setOutlineThickness(1);
    }
}

void LevelSelect::Update() {
    // Maintain non selected options white
    for (auto i = 0u; i < level_text_array.size(); ++i) {
        level_text_array[i].setFillColor(sf::Color::White);
    }

    level_text_array[selection % 3].setFillColor(sf::Color::Blue);
}

void LevelSelect::Draw(sf::RenderWindow& window) {
    window.clear();
    for (auto text : level_text_array) {
        window.draw(text);
    }
    window.draw(level_select);
    window.display();
}
