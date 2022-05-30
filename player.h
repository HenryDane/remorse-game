#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity.h"

#define PLAYER_DEFAULT_SPRITE 84

class Player : public Entity {
public:
    enum Facing {
        UP = 129, DOWN = 152, LEFT = 128, RIGHT = 153
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
