#include "stdafx.h"
#include "entity.h"
#include "player.h"

class PowerUp : public Entity {
public:
    PowerUp();
    void Draw(sf::RenderWindow& window);
    void Update(float countdown);

    void Invisibility(std::map<std::size_t, Player*>::const_iterator& iter);
    void Speed(std::map<std::size_t, Player*>::const_iterator& iter);
    void Rage(std::map<std::size_t, Player*>::const_iterator& iter);
    void Floaty(std::map<std::size_t, Player*>::const_iterator& iter);
    void Immunity(std::map<std::size_t, Player*>::const_iterator& iter);

    static void ResetPowerupEffects(std::map<std::size_t, Player*>::const_iterator& iter);

    float floating_effect = 7.f;
    std::size_t effect;
    bool alive = true;
};
