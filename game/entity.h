#pragma once

#include "stdafx.h"

class Entity {
public:
    void Load(std::string filename);
    void Draw(sf::RenderWindow& window);
    void Update(float elapsed_time);
    
    float GetWidth() const;
    float GetHeight() const;
    sf::Rect<float> GetBoundingRect() const;

    sf::RectangleShape rect;
    sf::Texture texture;
    sf::Sprite sprite;
    
    bool loaded = false;
};
