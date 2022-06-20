#ifndef _PROP_INVENTORY_H
#define _PROP_INVENTORY_H

#include <array>
#include <iostream>
#include "../item.h"

class PropertyInventory {
public:
    PropertyInventory(int num_max_slots, int inv_idx_start) {
        this->inv_idx_start = inv_idx_start;
        this->num_max_slots = num_max_slots;
        for (int i = 0; i < num_max_slots; i++) {
            items.push_back(Item());
        }
    };
    ~PropertyInventory() { };

    // inventoried-specific
    int get_num_items();
    Item& get_item_at(int idx, bool& found);
    bool add_item(Item item);
    bool remove_item(Item& item);
    bool remove_item(std::string name);
    bool remove_item(int idx);
private:
    std::vector<Item> items;
    int num_max_slots, inv_idx_start;
};

#endif // _PROP_INVENTORY_H
