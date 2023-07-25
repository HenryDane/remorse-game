#include "living.h"

EntityEffect::EntityEffect(EffectType type, int duration, float intensity) {
    this->type = type;
    this->duration = duration;
    this->intensity = intensity;
}

int EntityEffect::get_duration() {
    return duration;
}

float EntityEffect::get_intensity() {
    return intensity;
}

EntityEffect::EffectType EntityEffect::get_type() {
    return type;
}

void EntityEffect::dec_duration() {
    this->duration--;
}

bool EntityEffect::is_valid() {
    return this->duration > 0;
}

float PropertyLiving::get_max_health() {
    return max_health;
}

float PropertyLiving::get_health() {
    return health;
}

void PropertyLiving::set_health(float health) {
    this->health = health;
}

/*
    Tries to modify the entities health by dh. If the result is
    less than zero, it returns false, otherwise, it returns true.
    The resulting health is always >= 0.
*/
bool PropertyLiving::change_health(float dh) {
    this->health += dh;
    if (this->health < 0) {
        this->health = 0;
        return false;
    }

    return true;
}

void PropertyLiving::add_effect(int duration, EntityEffect::EffectType type,
    float intensity) {
    if (duration <= 0) return;
    this->effects.push_back(new EntityEffect(type, duration, intensity));
}

float PropertyLiving::get_attribute(EntityEffect::EffectType type) {
    float result = 0;

    for (EntityEffect* ef : effects) {
        if (ef->get_type() == type) {
            result += ef->get_intensity();
        }
    }

    return result;
}
