#include "aoe_entity.h"

int AOEEntity::get_intensity(EntityEffect::EffectType type) {
    switch(type) {
    case EntityEffect::EffectType::ATK:
        return atk;
    case EntityEffect::EffectType::DEF:
        return def;
    case EntityEffect::EffectType::SPD:
        return spd;
    case EntityEffect::EffectType::HP:
        return hp;
    }

    return 0;
}

int AOEEntity::get_lifetime() {
    return lifetime;
}
