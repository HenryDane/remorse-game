#include "trap.h"

void TrapEntity::press() {
    this->m_pressed = true;
    this->set_sprite(alt);
}

void TrapEntity::unpress() {
    this->m_pressed = false;
    this->set_sprite(type);
}

void TrapEntity::toggle() {
    if (this->m_pressed) {
        this->unpress();
    } else {
        this->press();
    }
}

bool TrapEntity::is_pressed() {
    return this->m_pressed;
}
