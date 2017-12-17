#pragma once

#include "stdafx.h"

class Entity {
public:
    Entity();
    virtual ~Entity();

    void Load(std::string filename);
    void Draw(sf::RenderWindow& window);
    void Update(float elapsed_time);
    
    float GetWidth() const;
    float GetHeight() const;
    sf::Rect<float> GetBoundingRect() const;

    b2Body* body;
    b2BodyDef bodydef;
    b2CircleShape circle_shape;
    b2PolygonShape shape;
    b2FixtureDef fixturedef;

    sf::RectangleShape rect;
    sf::CircleShape circ;
    sf::Texture texture;
    sf::Sprite sprite;

    bool loaded = false;
};
