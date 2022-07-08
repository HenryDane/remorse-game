#ifndef _DOOR_H
#define _DOOR_H

#include "entity.h"

class DoorEntity : public Entity {
public:
    enum TriggerType {
        NONE, ITEM, SWITCH
    };
    DoorEntity(float x, float y, int type, int alt, TriggerType trig_type, std::string& trigger, bool toggleable) :
        Entity(x, y, Entity::DOOR) {
        this->set_sprite(type);
        this->solid = true;

        this->type = type;
        this->alt = alt;
        this->trig_type = trig_type;
        this->trigger = trigger;
        this->toggleable = toggleable;

        this->m_open = false;
    }
    ~DoorEntity() { };

    // door stuff
    void open();
    void close();
    void toggle();
    bool is_open();
    bool is_toggleable();

    // handle triggers
    void on_trigger(Entity::Type type, std::string& name);

    // trigger information
    TriggerType get_trigger_type();
    std::string& get_trigger_value();
private:
    // rendering information
    int type, alt;

    // door information
    TriggerType trig_type;
    bool toggleable;
    std::string trigger;

    // state
    bool m_open;
};

#endif
