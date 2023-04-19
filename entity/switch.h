#ifndef _SWITCH_H
#define _SWITCH_H

#include "entity.h"

class SwitchEntity : public Entity {
public:
    SwitchEntity(float x, float y, int type, int alt, bool require_prev, std::string& prev, int reset_counter) :
        Entity(x, y, Entity::SWITCH) {
        this->set_sprite(type);
        this->solid = false;

        this->type = type;
        this->alt = alt;
        this->require_prev = require_prev;
        this->reset_counter = reset_counter;
        this->prev = prev;

        this->m_pressed = false;
    }
    ~SwitchEntity() { };

    // switch functions
    void press();
    void unpress();
    void toggle();
    bool is_pressed();
private:
    // render info
    int type, alt;

    // switch info
    std::string prev;
    int reset_counter;
    bool require_prev;

    // state info
    bool m_pressed;
};

#endif
