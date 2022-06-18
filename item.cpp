#include "item.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include "util.h"

/*
===============================================================================
    ITEM DATA
===============================================================================
*/

ItemData* _g_item_data_inst = nullptr;

struct _item_prop_t {
    int hp, atk, def, spd;

    _item_prop_t() {
        hp = 0; atk = 0; def = 0; spd = 0;
    }
};

_item_prop_t make_properties(std::string& str) {
    _item_prop_t ipt;
    std::vector<std::string> tokens = split_by_char(str, ',');
    std::vector<std::string> kvp;
    for (std::string& s : tokens) {
        kvp.clear();
        kvp = split_by_char(s, ':');
        if (kvp[0] == "HP") {
            ipt.hp = std::stoi(kvp[1]);
        } else if (kvp[0] == "DEF") {
            ipt.def = std::stoi(kvp[1]);
        } else if (kvp[0] == "ATK") {
            ipt.atk = std::stoi(kvp[1]);
        } else if (kvp[0] == "SPD") {
            ipt.spd = std::stoi(kvp[1]);
        }
    }
    return ipt;
}

ItemData::ItemData(std::string path) {
    if (_g_item_data_inst != nullptr) {
        std::cout << "[ITEMDATA] Duplicate item data instance found. Exiting..." << std::endl;
        exit(0);
    } else {
        _g_item_data_inst = this;
    }

    // add default def
    // TODO: generalize this
    item_defs[INVALID_ITEM_NAME] = {INVALID_ITEM_NAME, 373, ItemType::NONE,
                                    0, 0, 0, 0};

    // open file
    std::ifstream itemfile(path);
    if (!itemfile.is_open()) {
        std::cout << "ERROR: unable to open item file: " << path << std::endl;
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
        exit(1041);
    }

    // read line by line
    int line_num = 1;
    for (std::string line; getline(itemfile, line); ) {
        std::cout << line_num << "\t|" << line << std::endl;
        std::vector<std::string> tokens = split_by_char(line, ';');

        if (tokens.size() <= 0) {
            std::cout << "\t|SKIPPED" << std::endl;
            continue;
        }

        if (tokens[0] == "GENERIC") {
            // check we have right number of elements
            if (tokens.size() != 3) {
                std::cout << "ERROR: Malformed item definition on line: " << line_num << std::endl;
                exit(1042);
            }

            // make the item
            std::cout << "\t|ADD: " << tokens[1] << std::endl;
            item_defs[tokens[1]] = {tokens[1], std::stoi(tokens[2]), ItemType::ITEM,
                                    0, 0, 0, 0};
        } else if (tokens[0] == "ARMOR") {
            // check that we have the right number of elements
            if (tokens.size() != 4) {
                std::cout << "ERROR: Malformed item definition on line: " << line_num << std::endl;
                exit(1042);
            }

            // get the properties
            _item_prop_t props = make_properties(tokens[3]);

            // make the item
            std::cout << "\t|ADD: " << tokens[1] << std::endl;
            item_defs[tokens[1]] = {tokens[1], std::stoi(tokens[2]), ItemType::ARMOR,
                                    props.hp, props.atk, props.def, props.spd};
        } else if (tokens[0] == "WEAPON") {
            // check that we have the right number of elements
            if (tokens.size() != 4) {
                std::cout << "ERROR: Malformed item definition on line: " << line_num << std::endl;
                exit(1042);
            }

            // get the properties
            _item_prop_t props = make_properties(tokens[3]);

            // make the item
            std::cout << "\t|ADD: " << tokens[1] << std::endl;
            item_defs[tokens[1]] = {tokens[1], std::stoi(tokens[2]), ItemType::WEAPON,
                                    props.hp, props.atk, props.def, props.spd};
        } else if (tokens[0] == "CONSUMABLE") {
            // check that we have the right number of elements
            if (tokens.size() != 4) {
                std::cout << "ERROR: Malformed item definition on line: " << line_num << std::endl;
                exit(1042);
            }

            // get the properties
            _item_prop_t props = make_properties(tokens[3]);

            // make the item
            std::cout << "\t|ADD: " << tokens[1] << std::endl;
            item_defs[tokens[1]] = {tokens[1], std::stoi(tokens[2]), ItemType::CONSUMABLE,
                                    props.hp, props.atk, props.def, props.spd};
        }

        line_num++;
    }
}

ItemData::~ItemData() {

}

// create items
Item ItemData::make_item(std::string name) {
    if (is_item_defined(name)) {
        return Item(name, 1, item_defs[name].type);
    } else {
        return Item();
    }
}

Item ItemData::make_item(std::string name, int n) {
    if (is_item_defined(name)) {
        return Item(name, n, item_defs[name].type);
    } else {
        return Item();
    }
}

// item info
int ItemData::get_sprite(Item& item) {
    if (!is_item_defined(item.get_name())) {
        std::cout << "item name=" << item.get_name() << std::endl;
        item.make_invalid(); // since we got a bad item, reset it
        return item_defs[INVALID_ITEM_NAME].sprite;
    }
    return item_defs[item.get_name()].sprite;
}

bool ItemData::is_item_defined(std::string name) {
    return item_defs.find(name) != item_defs.end();
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

Item::Item(std::string name, int n, ItemType type) {
    this->t = type;
    this->n = n;
    this->name = name;
}

Item::~Item() {

}

ItemType Item::get_type() {
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

bool Item::is_stackable(Item& item) const {
    // TODO: remove this if
    if ((this->t == ItemType::ARMOR) | (this->t == ItemType::WEAPON)) {
        return false;
    }
    return (this->t == item.get_type()) &&
        (this->name.compare(item.get_name()) == 0) &&
        (this->n + item.get_count() <= 999);
}

void Item::make_invalid() {
    this->t = ItemType::NONE;
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

bool ItemEntity::is_pickup_ok() {
    return pickup_remaining < 0;
}

void ItemEntity::make_invalid() {
    this->item.make_invalid();
    this->set_sprite(ItemData::inst().get_sprite(item));
}

void ItemEntity::animate(float dt) {
    const float pi = 3.14159;

    // update time
    time += dt;
    time = time - (long) time;

    this->set_render_offset_y(0.2 * sinf(2 * pi * time));

    // update pickup time
    pickup_remaining -= dt;
}
