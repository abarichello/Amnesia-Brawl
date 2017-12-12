#include "mode_select.h"
#include "level_select.h"

ModeSelect::ModeSelect() {
    if(!arial.loadFromFile(ARIEL_PIXEL_FONT)) {
        std::cout << "Error loading arial font!" << "\n";
    }
    if(!banksia.loadFromFile(BANKSIA_FONT)) {
        std::cout << "Error loading banksia font!" << "\n";
    }

    player_select.setFont(arial);
    player_select.setOutlineColor(sf::Color::White);
    player_select.setOutlineThickness(3);
    player_select.setFillColor(sf::Color(227, 12, 18));
    player_select.setCharacterSize(43);
    player_select.setString("Choose the gamemode:");
    player_select.setOrigin(player_select.getLocalBounds().width / 2, player_select.getLocalBounds().height / 2);
    player_select.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 3 - GAME_HEIGHT / 6);

    two_players.setFont(banksia);
    two_players.setFillColor(sf::Color::Black);
    two_players.setCharacterSize(35);
    two_players.setString("2 PLAYERS");
    two_players.setPosition(GAME_WIDTH / 14, GAME_HEIGHT / 3);

    three_players.setFont(banksia);
    three_players.setFillColor(sf::Color::Black);
    three_players.setCharacterSize(35);
    three_players.setString("3 PLAYERS");
    three_players.setPosition(GAME_WIDTH / 14, GAME_HEIGHT / 3 + two_players.getLocalBounds().height);

    four_players.setFont(banksia);
    four_players.setFillColor(sf::Color::Black);
    four_players.setCharacterSize(35);
    four_players.setString("4 PLAYERS");
    four_players.setPosition(GAME_WIDTH / 14, GAME_HEIGHT / 3  + two_players.getLocalBounds().height * 2);

    player_text_array.push_back(two_players);
    player_text_array.push_back(three_players);
    player_text_array.push_back(four_players);

    // Common txt configs
    for (auto txt : player_text_array) {
        txt.setOutlineColor(sf::Color::White);
        txt.setOutlineThickness(1);
    }
}

void ModeSelect::Update() {
    for (auto j = 0u; j < player_text_array.size(); ++j) {
        player_text_array[j].setFillColor(sf::Color::White);
    }
    player_text_array[selection % 3].setFillColor(sf::Color::Blue);
}

void ModeSelect::Draw(sf::RenderWindow& window) {
    window.clear();
    for (auto text : player_text_array) {
        window.draw(text);
    }
    window.draw(player_select);
    window.display();
}
