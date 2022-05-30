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

float Player::get_render_x() {
    return get_x() + this->get_render_offset_x();
}

float Player::get_render_y() {
    return get_y() + this->get_render_offset_y();
}

void Player::animate(float dt) {
    const float epsilon = 0.1;

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
