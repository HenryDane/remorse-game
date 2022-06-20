#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity/entity.h"
#include "config.h"
#include "entity/item.h"
#include "entity/prop/living.h"
#include "entity/prop/inventory.h"

class Player : public Entity, public PropertyLiving,
    public PropertyInventory {
public:
    enum Facing {
        UP = PLAYER_FACING_UP,
        DOWN = PLAYER_FACING_DOWN,
        LEFT = PLAYER_FACING_LEFT,
        RIGHT = PLAYER_FACING_RIGHT
    };
    Player(int x, int y, float health) :
        Entity(x, y, Entity::PLAYER),
        PropertyLiving(health, health),
        PropertyInventory(PLAYER_NUM_TOTAL_INV, PLAYER_INV_IDX_START) {
        this->set_sprite(PLAYER_DEFAULT_SPRITE);
        this->facing = Player::DOWN;
    }
    ~Player();

    // moveable-specific
    void set_dxdy(int dx, int dy);
    Facing get_facing();
    void set_facing(Facing facing);
    void set_facing(int dx, int dy);

    // special rendering stuff
    float get_render_x();
    float get_render_y();

    void animate(float dt);
private:
    Facing facing;
    float time;
    int dx, dy;
};

#endif
