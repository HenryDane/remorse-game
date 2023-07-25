#ifndef _AOE_ENTITY_H
#define _AOE_ENTITY_H

#include "entity.h"
#include "prop/living.h"

class AOEEntity : public Entity {
public:
    AOEEntity(float x, float y, int lifetime, int atk, int def, int spd,
        int hp) : Entity(x, y, Entity::AOE) {
        this->atk = atk;
        this->def = def;
        this->spd = spd;
        this->hp = hp;
        this->lifetime = lifetime;

        this->set_sprite(91);
    }

    int get_intensity(EntityEffect::EffectType type);
    int get_lifetime();
private:
    int atk, def, spd, hp, lifetime;
};

#endif
