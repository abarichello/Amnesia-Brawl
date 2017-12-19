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
#include "mode_select.h"
#include "timer.h"

#include <utility>

class Game {
public:
    Game();
    void Start();
    void LoadResources();
    void CreateRound(std::size_t players_num, std::size_t level_number, b2World& world);
    void LoadPlayers(std::size_t number_of_players);
    void SpawnPlayer(std::size_t number, std::shared_ptr<Player> player, sf::Color color, sf::Keyboard::Key jump, sf::Keyboard::Key left, sf::Keyboard::Key right);
    void CreatePlayer(b2World& world, std::shared_ptr<Player> player, int x, int y);

    void TitleScreen();
    void ModeSelect();
    void LevelSelect();
    void GameLoop(sf::Time elapsed_time, float& countdown, Timer& powerup_clock);

    void WinnerCheck();
    void ResetPowerups();
    void EndRound();

    sf::RenderWindow window;
    sf::View game_view;
    sf::Clock global_clock;
    sf::Text match_clock;

    enum GameState {
        STATE_TITLE,
        STATE_MODE_SELECT,
        STATE_LEVEL_SELECT,
        STATE_PLAY,
        STATE_PAUSE
    };

    GameState game_state = GameState::STATE_TITLE;

    b2Vec2 gravity;
    b2World world;
    HUD* hud;
    class TitleScreen* title_screen;
    class ModeSelect* mode_select;
    class LevelSelect* levelselect;
    
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    std::shared_ptr<Player> player3;
    std::shared_ptr<Player> player4;
    Map* map;
    
    sf::Texture control_texture;
    sf::RectangleShape control_shape;
    sf::Color amnesia_blue;
    sf::Color amnesia_red;
    sf::Color amnesia_dark_blue;
    sf::Color amnesia_dark_red;

    sf::SoundBuffer spring_buffer;
    sf::SoundBuffer squash_buffer;
    sf::Sound spring_sound;
    sf::Sound squash_sound;

    std::vector<PowerUp> powerup_array;
    std::vector<b2Vec2> spawn_locations;

    static GameObjectManager _game_object_manager;
};
