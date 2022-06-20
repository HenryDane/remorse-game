#ifndef _CHEST_H
#define _CHEST_H

#include "entity.h"

class ChestEntity : public Entity {
public:
    ChestEntity(float x, float y, int type, std::string& loot) :
        Entity(x, y, Entity::CHEST) {
        this->set_sprite(type);
        this->solid = true;
        // TODO: parse loot
    }
    ~ChestEntity() { }

    std::string get_interact_name() const;
};

#endif // _CHEST_H
