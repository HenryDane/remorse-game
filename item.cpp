#include "item.h"
#include <cmath>
#include <iostream>

/*
===============================================================================
    ITEM DATA
===============================================================================
*/

ItemData* _g_item_data_inst = nullptr;

ItemData::ItemData(std::string path) {
    if (_g_item_data_inst != nullptr) {
        std::cout << "[ITEMDATA] Duplicate item data instance found. Exiting..." << std::endl;
        exit(0);
    } else {
        _g_item_data_inst = this;
    }

    item_defs[INVALID_ITEM_NAME] = {INVALID_ITEM_NAME, 373, 0, 0, 0, 0};
    item_defs["Wheat Seeds"]     = {"Wheat Seeds"    , 797, 0, 0, 0, 0};
}

ItemData::~ItemData() {

}

// create items
Item ItemData::make_item(std::string name) {

}

Item ItemData::make_item(std::string name, int n) {
    return Item(ItemData::ITEM, n, name);
}

// item info
int ItemData::get_sprite(Item& item) {
    return item_defs[item.get_name()].sprite;
}

ItemData& ItemData::inst() {
    return *_g_item_data_inst;
}

/*
===============================================================================
    ITEM
===============================================================================
*/

Item::Item() {
    this->make_invalid();
}

Item::Item(ItemData::Type t, int n, std::string name) {
    this->t = t;
    this->n = n;
    this->name = name;
}

Item::~Item() {

}

ItemData::Type Item::get_type() {
    return t;
}

int Item::get_count() {
    return this->n;
}

void Item::set_count(int n) {
    this->n = n;
}

std::string Item::get_name() {
    return this->name;
}

bool Item::is_equal(Item& item) const {
    return (this->t == item.get_type()) &&
        (this->n == item.get_count()) &&
        (this->name.compare(item.get_name()) == 0);
}

void Item::make_invalid() {
    this->t = ItemData::NONE;
    this->n = 0;
    this->name = INVALID_ITEM_NAME;
}

/*
===============================================================================
    ITEM ENTITY
===============================================================================
*/

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
