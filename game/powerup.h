#include "stdafx.h"
#include "entity.h"

class PowerUp : public Entity {
public:
    PowerUp();
    void Draw(sf::RenderWindow& window);
    void Update(sf::Time elapsed_time);

    void Explosion();

    std::size_t effect;
    bool alive = true;
};
