#include "hud.h"

HUD::HUD(std::size_t number_of_players) {
    if (!hud_font.loadFromFile(HUD_FONT)) {
        std::cout << "Error loading font!" << "\n";
    }
    if (!bonbon_font.loadFromFile(BONBON_FONT)) {
        std::cout << "Error loading bonbon!" << "\n";
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
    if (number_of_players > 2 && number_of_players < 5) {
        score_array[2].setFillColor(sf::Color(29, 12, 137));
        score_array[2].setPosition(score.getLocalBounds().width / 2, score.getLocalBounds().height / 2);
        if (number_of_players == 4) {
            score_array[3].setFillColor(sf::Color(158, 0, 0));
            score_array[3].setPosition(GAME_WIDTH - score.getLocalBounds().width * 5 / 2, score.getLocalBounds().height / 2);
        }
    }

    match_clock.setFont(hud_font);
    match_clock.setOutlineColor(sf::Color::White);
    match_clock.setOutlineThickness(2);
    match_clock.setFillColor(sf::Color(14, 77, 203));
    match_clock.setCharacterSize(25);
    match_clock.setString("0:00"); // placeholder string
    match_clock.setPosition(GAME_WIDTH/2 - match_clock.getLocalBounds().width / 2, match_clock.getLocalBounds().height);

    winner_text.setFont(bonbon_font);
    winner_text.setOutlineColor(sf::Color::Black);
    winner_text.setOutlineThickness(3);
    winner_text.setFillColor(sf::Color(227, 12, 18));
    winner_text.setCharacterSize(50);
    winner_text.setString("PX WON"); // placeholder string
}

void HUD::Update(std::map<std::size_t, Player*>::const_iterator &iter, float countdown) {
    for (auto i = 0u; i < number_of_players; ++i) {
        score_array[i].setString(std::to_string(iter->second->score));
        ++iter;
    }
    auto minutes = std::to_string((int)countdown / 60);
    auto seconds = std::to_string((int)countdown % 60);
    if (std::stoi(seconds) < 10) {
        seconds = "0" + seconds;
        match_clock.setFillColor(sf::Color(227, 12, 18));
    }

    if (countdown < 0) {
        match_clock.setString("-END-");
    } else {
        match_clock.setString(minutes + ":" + seconds);
    }
}

void HUD::Draw(sf::RenderWindow& window) {
    for (auto score : score_array) {
        window.draw(score);
    }
    window.draw(match_clock);
}
