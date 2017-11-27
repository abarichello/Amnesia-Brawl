#pragma once

#include "player.h"

class GameObjectManager {
public:
    GameObjectManager();
    ~GameObjectManager();
    
    void Add(std::string name, Player* game_object);
    void Remove(std::string name);
    int GetObjectCount() const;
    Player* Get(std::string name) const;
    static sf::Clock& GetClock();

    void DrawAll(sf::RenderWindow& render_window);
    std::map<std::string, Player*> _game_objects;
    
private:
    static sf::Clock _clock;
    
    struct GameObjectDeallocator {
        void operator() (const std::pair<std::string, Player*>& p) const {
            delete p.second;
        }
    };
};