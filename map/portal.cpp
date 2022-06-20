#include "portal.h"

Portal::Portal(std::string& name, int x, int y, int w, int h, int tx, int ty) {
    this->s = name;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->tx = tx;
    this->ty = ty;
}

std::string& Portal::get_name() {
    return s;
}

int Portal::get_x() {
    return x;
}

int Portal::get_y() {
    return y;
}

int Portal::get_w() {
    return w;
}

int Portal::get_h() {
    return h;
}

int Portal::get_target_x() {
    return tx;
}

int Portal::get_target_y() {
    return ty;
}
