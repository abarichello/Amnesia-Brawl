#pragma once

#include "player.h"

class GameObjectManager {
public:
    GameObjectManager();
    ~GameObjectManager();
    
    void Add(std::size_t number, std::shared_ptr<Player> game_object);
    void Remove(std::size_t number);
    int GetObjectCount() const;
    std::shared_ptr<Player> Get(std::size_t number) const;
    static sf::Clock& GetClock();

    void DrawAll(sf::RenderWindow& render_window);
    std::map<std::size_t, std::shared_ptr<Player>> _game_objects;
    
private:
    static sf::Clock _clock;
};