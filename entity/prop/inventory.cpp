#include "inventory.h"

int PropertyInventory::get_num_items() {
    return num_max_slots;
}

Item& PropertyInventory::get_item_at(int idx, bool& found) {
    found = (idx > 0 && idx < num_max_slots);
    return items[idx];
}

/*
    This function will attempt to add an item to the players inventory slots.
    It will never try to add an item to the quickslots nor the equippable
    slots. Additionally, the function will fail if there is no room.
*/
bool PropertyInventory::add_item(Item item) {
    for (int i = this->inv_idx_start; i < this->get_num_items(); i++) {
        if (items[i].is_stackable(item)) {
            items[i].set_count(items[i].get_count() + item.get_count());
            return true;
        }
    }
    for (int i = this->inv_idx_start; i < this->get_num_items(); i++) {
        if (items[i].get_type() == ItemType::NONE) {
            items[i] = item;
            return true;
        }
    }

    return false;
}

/*
    This function removes an item from the players inventory. Only the first
    occurrence is removed, and it must match count, name, and type exactly.
*/
bool PropertyInventory::remove_item(Item& item) {
    for (int i = num_max_slots - 1; i >= 0 ; i--) {
        if (items[i].is_equal(item)) {
            items[i].make_invalid();
            return true;
        }
    }

    return false;
}

bool PropertyInventory::remove_item(std::string name) {
    for (int i = num_max_slots - 1; i >= 0 ; i--)  {
        if (items[i].get_name().compare(name) == 0) {
            items[i].make_invalid();
            return true;
        }
    }

    return false;
}

bool PropertyInventory::remove_item(int idx) {
    if (idx > 0 && idx < num_max_slots) {
        items[idx].make_invalid();
        return true;
    } else {
        return false;
    }
}
