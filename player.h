#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity.h"
#include "config.h"

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

    // player-specific
    float get_health();
    void set_health(float health);
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
    float health;
    float time;
    int dx, dy;
};

#endif
