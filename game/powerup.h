#include "stdafx.h"
#include "entity.h"
#include "player.h"

class PowerUp : public Entity {
public:
    PowerUp(b2World& world);
    // ~PowerUp();
    void Draw(sf::RenderWindow& window);
    void Update(sf::Time elapsed_time);

    void Speed(std::map<std::size_t, Player*>::const_iterator& iter);

    void Floaty(std::map<std::size_t, Player*>::const_iterator& iter);

    void ResetPowerupEffects(std::map<std::size_t, Player*>::const_iterator& iter);

    std::size_t effect;
    bool alive = true;
};
