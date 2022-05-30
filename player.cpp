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
    this->set_render_offset_x(-dx);
    this->set_render_offset_y(-dy);
    this->dx = dx;
    this->dy = dy;
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

    if (rx * dx < 0) {
        this->set_render_offset_x(rx + dt * dx);
    }
    if (ry * dy < 0) {
        this->set_render_offset_y(ry + dt * dy);
    }
}
