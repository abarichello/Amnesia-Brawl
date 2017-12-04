#include "title_screen.h"

TitleScreen::TitleScreen() {
    if (!bonbon_font.loadFromFile(BONBON_FONT)) {
        std::cout << "Error loading bonbon!" << "\n";
    }
    if (!square_font.loadFromFile(SQUARE_FONT)) {
        std::cout << "Error loading squarefont!" << "\n";
    }
    if (!linebrk_font.loadFromFile(LINE_BRK_FONT)) {
        std::cout << "Error loading linebrkfont!" << "\n";
    }

    game_title.setFont(bonbon_font);
    game_title.setOutlineColor(sf::Color::White);
    game_title.setOutlineThickness(4);
    game_title.setFillColor(sf::Color(227, 12, 18));
    game_title.setCharacterSize(170);
    game_title.setString("Amnesia");
    game_title.setOrigin(game_title.getLocalBounds().width / 2, game_title.getLocalBounds().height / 2);
    game_title.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 3);

    game_subtitle.setFont(square_font);
    game_subtitle.setOutlineColor(sf::Color::White);
    game_subtitle.setOutlineThickness(1);
    game_subtitle.setFillColor(sf::Color(29, 12, 137));
    game_subtitle.setCharacterSize(40);
    game_subtitle.setString("brawl");
    game_subtitle.setOrigin(game_subtitle.getLocalBounds().width / 2, game_subtitle.getLocalBounds().height / 2);
    game_subtitle.setPosition(GAME_WIDTH / 2 + GAME_WIDTH/6, GAME_HEIGHT / 3 + GAME_HEIGHT/7);

    press_space.setFont(linebrk_font);
    press_space.setFillColor(sf::Color::White);
    press_space.setCharacterSize(40);
    press_space.setString("PRESS SPACE");
    press_space.setOrigin(press_space.getLocalBounds().width / 2, press_space.getLocalBounds().height / 2);
    press_space.setPosition(GAME_WIDTH / 2, GAME_HEIGHT - GAME_HEIGHT/4);
}

void TitleScreen::Draw(sf::RenderWindow& window) {
    window.clear();

    window.draw(game_title);
    window.draw(game_subtitle);
    window.draw(press_space);

    window.display();
}
