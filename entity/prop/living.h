#ifndef _PROP_LIVING_H
#define _PROP_LIVING_H

#include <vector>

class EntityEffect {
public:
    enum EffectType {
        ATK, DEF, SPD, HP
    };

    EntityEffect(EffectType type, int duration, float intensity);

    int get_duration();
    float get_intensity();
    EffectType get_type();

    void dec_duration();
    bool is_valid();
private:
    int duration;
    EffectType type;
    float intensity;
};

class PropertyLiving {
public:
    PropertyLiving(float health, float max_health) {
        this->health = health;
        this->max_health = max_health;
    }
    ~PropertyLiving() { };

    float get_max_health();
    float get_health();
    void set_health(float health);
    bool change_health(float dh);

    void add_effect(int duration, EntityEffect::EffectType type, float intensity);
    float get_attribute(EntityEffect::EffectType type);
private:
    float health, max_health;
    std::vector<EntityEffect*> effects;
};

#endif // _PROP_LIVING_H
