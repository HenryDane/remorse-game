#include "switch.h"

// switch functions
void SwitchEntity::press() {
    this->m_pressed = true;
    this->set_sprite(alt);
}

void SwitchEntity::unpress() {
    this->m_pressed = false;
    this->set_sprite(type);
}

void SwitchEntity::toggle() {
    if (this->m_pressed) {
        this->unpress();
    } else {
        this->press();
    }
}

bool SwitchEntity::is_pressed() {
    return m_pressed;
}

void SwitchEntity::on_trigger(Entity::Type type, std::string& name) {
    // TODO: make switch handle triggers
}
