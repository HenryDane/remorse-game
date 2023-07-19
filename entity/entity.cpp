#include "entity.h"
#include "../config.h"

Entity::Entity() {
    Entity(-1, -1, Type::NONE);
}

Entity::Entity(float x, float y, Type t) {
    this->x = x;
    this->y = y;
    this->t = t;
    this->rx = 0.0f;
    this->ry = 0.0f;
    this->solid = false;
    this->interactable = false;
    this->build_vert_array();
}

Entity::~Entity() {

}

float Entity::get_x() const {
    return this->x;
}

float Entity::get_y() const {
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

Entity::Type Entity::get_type() const {
    return this->t;
}

bool Entity::is_solid() const {
    return solid;
}

bool Entity::is_interactable() const {
    return interactable;
}

std::string Entity::get_interact_name() const {
    return "<invalid entity>";
}

void Entity::do_interact() {
    return;
}

int Entity::get_sprite() const {
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
    float u = (sprite % SHEET_WIDTH) * TILE_SIZE;
    float v = (sprite / SHEET_WIDTH) * TILE_SIZE;

    float mx, my;
    if (this->t == Entity::PLAYER) {
        mx = 10 + rx;
        my = 7 + ry;
    } else {
        mx = x + rx;
        my = y + ry;
    }

    vertices[0] = sf::Vertex(sf::Vector2f(mx * TILE_RENDER_SIZE, my * TILE_RENDER_SIZE), sf::Vector2f(u, v));
    vertices[1] = sf::Vertex(sf::Vector2f((mx + 1) * TILE_RENDER_SIZE, my * TILE_RENDER_SIZE), sf::Vector2f(u + TILE_SIZE, v));
    vertices[2] = sf::Vertex(sf::Vector2f((mx + 1) * TILE_RENDER_SIZE, (my + 1) * TILE_RENDER_SIZE), sf::Vector2f(u + TILE_SIZE, v + TILE_SIZE));
    vertices[3] = sf::Vertex(sf::Vector2f(mx * TILE_RENDER_SIZE, (my + 1) * TILE_RENDER_SIZE), sf::Vector2f(u, v + TILE_SIZE));
}
