#pragma once

#include "stdafx.h"
#include "player.h"
#include "obstacle.h"
#include "game_object_manager.h"
#include "hud.h"
#include "powerup.h"
#include "map.h"
#include "title_screen.h"
#include "level_select.h"

#include <utility>

class Game {
public:
    Game();
    void Start();
    void LoadResources();
    void CreateRound(std::size_t players_num, std::size_t level_number, b2World& world);
    void LoadPlayers(std::size_t number_of_players);
    void SpawnPlayer(std::size_t number, Player* player, sf::Color color, sf::Keyboard::Key jump, sf::Keyboard::Key left, sf::Keyboard::Key right);
    void CreatePlayer(b2World& world, Player* player, int x, int y);

    void TitleScreen();
    void LevelSelect();
    void GameLoop(float& countdown, sf::Clock& powerup_clock);

    void WinnerCheck();
    void ResetPowerups();
    void EndRound();

    sf::RenderWindow window;
    sf::View game_view;
    sf::Clock global_clock;
    sf::Text match_clock;

    enum GameState {
        STATE_TITLE,
        STATE_LEVEL_SELECT,
        STATE_PLAY,
        STATE_PAUSE
    };

    GameState game_state = GameState::STATE_TITLE;

    b2Vec2 gravity;
    b2World world;
    HUD* hud;
    class TitleScreen* title_screen;
    class LevelSelect* levelselect;
    
    Player* player1;
    Player* player2;
    Player* player3;
    Player* player4;
    Map* map;
    
    sf::Color amnesia_blue;
    sf::Color amnesia_red;
    sf::Color amnesia_dark_blue;
    sf::Color amnesia_dark_red;

    sf::SoundBuffer sound_buffer;
    sf::Music background_music;

    std::vector<PowerUp> powerup_array;
    std::vector<b2Vec2> spawn_locations;

    static GameObjectManager _game_object_manager;
};
