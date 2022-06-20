#include "living.h"

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
