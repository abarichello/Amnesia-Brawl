#pragma once

#include "player.h"

class GameObjectManager {
public:
    GameObjectManager();
    ~GameObjectManager();
    
    void Add(std::size_t number, Player* game_object);
    void Remove(std::size_t number);
    int GetObjectCount() const;
    Player* Get(std::size_t number) const;
    static sf::Clock& GetClock();

    void DrawAll(sf::RenderWindow& render_window);
    std::map<std::size_t, Player*> _game_objects;
    
private:
    static sf::Clock _clock;
    
    struct GameObjectDeallocator {
        void operator() (const std::pair<std::size_t, Player*>& p) const {
            delete p.second;
        }
    };
};