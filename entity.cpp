#include "entity.h"

Entity::Entity() {
    this->x = -1;
    this->y = -1;
    this->t = Entity::NONE;
    this->rx = 0.0f;
    this->ry = 0.0f;
    this->build_vert_array();
}

Entity::Entity(float x, float y, Type t) {
    this->x = x;
    this->y = y;
    this->t = t;
    this->rx = 0.0f;
    this->ry = 0.0f;
    this->build_vert_array();
}

Entity::~Entity() {

}

float Entity::get_x() {
    return this->x;
}

float Entity::get_y() {
    return this->y;
}

void Entity::set_x(float x) {
    this->x = x;
    this->build_vert_array();
}

void Entity::set_y(float y) {
    this->y = y;
    this->build_vert_array();
}

Entity::Type Entity::get_type() {
    return this->t;
}

int Entity::get_sprite() {
    return this->sprite;
}

void Entity::set_sprite(int sprite) {
    this->sprite = sprite;
    this->build_vert_array();
}

void Entity::animate(float dt) {
    return;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

    // draw the vertex array
    target.draw(vertices, 4, sf::Quads, states);
}

void Entity::set_render_offset_x(float x) {
    this->rx = x;
    this->build_vert_array();
}

void Entity::set_render_offset_y(float y) {
    this->ry = y;
    this->build_vert_array();
}

float Entity::get_render_offset_x() {
    return this->rx;
}

float Entity::get_render_offset_y() {
    return this->ry;
}

void Entity::build_vert_array() {
    float u = (sprite % 24) * 16;
    float v = (sprite / 24) * 16;

    float mx, my;
    if (this->t == Entity::PLAYER) {
        mx = 10 + rx;
        my = 7 + ry;
    } else {
        mx = x + rx;
        my = y + ry;
    }

    vertices[0] = sf::Vertex(sf::Vector2f(mx * 32.0f, my * 32.0f), sf::Vector2f(u, v));
    vertices[1] = sf::Vertex(sf::Vector2f((mx + 1) * 32.0f, my * 32.0f), sf::Vector2f(u + 16, v));
    vertices[2] = sf::Vertex(sf::Vector2f((mx + 1) * 32.0f, (my + 1) * 32.0f), sf::Vector2f(u + 16, v + 16));
    vertices[3] = sf::Vertex(sf::Vector2f(mx * 32.0f, (my + 1) * 32.0f), sf::Vector2f(u, v + 16));
}
