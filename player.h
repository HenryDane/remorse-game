#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity.h"
#include "config.h"
#include "item.h"

#define PLAYER_INV_IDX_START 7
#define NUM_PLAYER_INV_SLOTS 5
#define NUM_PLAYER_INV_TOTAL 7 + NUM_PLAYER_INV_SLOTS

class Player : public Entity {
public:
    enum Facing {
        UP = PLAYER_FACING_UP,
        DOWN = PLAYER_FACING_DOWN,
        LEFT = PLAYER_FACING_LEFT,
        RIGHT = PLAYER_FACING_RIGHT
    };
    Player(int x, int y, float health) : Entity(x, y, Entity::PLAYER) {
        this->health = health;
        this->set_sprite(PLAYER_DEFAULT_SPRITE);
        this->facing = Player::DOWN;
    }
    ~Player();

    // living-specific
    float get_health();
    void set_health(float health);

    // moveable-specific
    void set_dxdy(int dx, int dy);
    Facing get_facing();
    void set_facing(Facing facing);
    void set_facing(int dx, int dy);

    // inventoried-specific
    int get_num_items();
    Item& get_item_at(int idx, bool& found);
    bool add_item(Item item);
    bool remove_item(Item& item);
    bool remove_item(std::string name);
    bool remove_item(int idx);

    // special rendering stuff
    float get_render_x();
    float get_render_y();

    void animate(float dt);
private:
    Facing facing;
    float health;
    float time;
    int dx, dy;
    Item items[NUM_PLAYER_INV_TOTAL];
};

#endif
