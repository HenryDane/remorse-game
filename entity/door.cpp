#include "door.h"

std::string DoorEntity::get_interact_name() const {
    if (this->m_open) {
        return "Close Door";
    } else {
        return "Open Door";
    }
}

void DoorEntity::open() {
    this->m_open = true;
    this->solid = false;
    this->set_sprite(alt);
}

void DoorEntity::close() {
    this->m_open = false;
    this->solid = true;
    this->set_sprite(type);
}

void DoorEntity::toggle() {
    if (this->m_open) {
        this->close();
    } else {
        this->open();
    }
}

bool DoorEntity::is_open() {
    return m_open;
}

bool DoorEntity::is_toggleable() {
    return toggleable;
}

DoorEntity::TriggerType DoorEntity::get_trigger_type() {
    return trig_type;
}

std::string& DoorEntity::get_trigger_value() {
    return trigger;
}
