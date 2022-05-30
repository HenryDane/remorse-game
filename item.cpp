#include "item.h"
#include<cmath>

Item::Item(Type t) {
    this->t = t;
    this->n = 1;
}

Item::Item(Type t, int n) {
    this->t = t;
    this->n = n;
}

Item::~Item() {

}

Item::Type Item::get_type() {
    return t;
}

int Item::get_count() {
    return this->n;
}

void Item::set_count(int n) {
    this->n = n;
}

ItemEntity::~ItemEntity() {

};

Item& ItemEntity::get_item() {
    return this->item;
}

void ItemEntity::animate(float dt) {
    const float pi = 3.14159;

    // update time
    time += dt;
    time = time - (long) time;

    this->set_render_offset_y(0.2 * sinf(2 * pi * time));
}
