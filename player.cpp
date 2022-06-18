#include "player.h"

Player::~Player() {

}

float Player::get_health() {
    return this->health;
}

void Player::set_health(float health) {
    this->health = health;
}

void Player::set_dxdy(int dx, int dy) {
    this->set_x(this->get_x() + dx);
    this->set_y(this->get_y() + dy);
    this->set_render_offset_x(-dx/2.0f);
    this->set_render_offset_y(-dy/2.0f);
    this->dx = dx;
    this->dy = dy;
}

Player::Facing Player::get_facing() {
    return this->facing;
}

void Player::set_facing(Facing facing) {
    this->facing = facing;
    this->set_sprite(this->facing);
}

void Player::set_facing(int dx, int dy) {
    if (dx == 1 && dy == 0) {
        this->facing = Player::RIGHT;
    } else if (dx == -1 && dy == 0) {
        this->facing = Player::LEFT;
    } else if (dx == 0 && dy == 1) {
        this->facing = Player::DOWN;
    } else if (dx == 0 && dy == -1) {
        this->facing = Player::UP;
    } else {
        this->facing = Player::DOWN;
    }

    this->set_sprite(this->facing);
}

int Player::get_num_items() {
    return NUM_PLAYER_INV_TOTAL;
}

Item& Player::get_item_at(int idx, bool& found) {
    found = (idx > 0 && idx < NUM_PLAYER_INV_TOTAL);
    return items[idx];
}

/*
    This function will attempt to add an item to the players inventory slots.
    It will never try to add an item to the quickslots nor the equippable
    slots. Additionally, the function will fail if there is no room.
*/
bool Player::add_item(Item item) {
    for (int i = PLAYER_INV_IDX_START; i < this->get_num_items(); i++) {
        if (items[i].is_stackable(item)) {
            items[i].set_count(items[i].get_count() + item.get_count());
            return true;
        }
    }
    for (int i = PLAYER_INV_IDX_START; i < this->get_num_items(); i++) {
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
bool Player::remove_item(Item& item) {
    for (int i = NUM_PLAYER_INV_TOTAL - 1; i >= 0 ; i--) {
        if (items[i].is_equal(item)) {
            items[i].make_invalid();
            return true;
        }
    }

    return false;
}

bool Player::remove_item(std::string name) {
    for (int i = NUM_PLAYER_INV_TOTAL - 1; i >= 0 ; i--)  {
        if (items[i].get_name().compare(name) == 0) {
            items[i].make_invalid();
            return true;
        }
    }

    return false;
}

bool Player::remove_item(int idx) {
    if (idx > 0 && idx < NUM_PLAYER_INV_TOTAL) {
        items[idx].make_invalid();
        return true;
    } else {
        return false;
    }
}

float Player::get_render_x() {
    return get_x() + this->get_render_offset_x();
}

float Player::get_render_y() {
    return get_y() + this->get_render_offset_y();
}

void Player::animate(float dt) {
    // speed it up
    dt *= 6;

    // update time
    time += dt;
    time = time - (long) time;

    // get offset
    float rx = this->get_render_offset_x();
    float ry = this->get_render_offset_y();

    // set the offsets
    if (rx * dx < 0) {
        this->set_render_offset_x(rx + dt * dx);
    }
    if (ry * dy < 0) {
        this->set_render_offset_y(ry + dt * dy);
    }
}
