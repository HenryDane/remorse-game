#ifndef _TRAP_H
#define _TRAP_H

#include "entity.h"

class TrapEntity : public Entity {
public:
    TrapEntity(float x, float y, int type, int alt, int damage, int dx, int dy,
        bool has_arm_trigger, std::string& arm_trigger_name,
        bool has_disarm_trigger, std::string& disarm_trigger_name) :
        Entity(x, y, Entity::TRAP) {
        this->set_sprite(type);
        this->solid = false;

        this->type = type;
        this->alt = alt;
        this->damage = damage;
        this->dx = dx;
        this->dy = dy;
        this->has_arm_trigger = has_arm_trigger;
        this->has_disarm_trigger = has_disarm_trigger;
        this->arm_trigger = arm_trigger_name;
        this->disarm_trigger = disarm_trigger_name;

        this->m_pressed = false;
    }

    // trap functions
    void press();
    void unpress();
    void toggle();
    bool is_pressed();

private:
    // render information
    int type, alt;

    // trap information
    std::string arm_trigger, disarm_trigger;
    int damage, dx, dy;
    bool has_arm_trigger, has_disarm_trigger;

    // state information
    bool m_pressed = false;
};

#endif
