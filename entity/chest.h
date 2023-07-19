#ifndef _CHEST_H
#define _CHEST_H

#include "entity.h"
#include "prop/inventory.h"

class ChestEntity : public Entity, public PropertyInventory {
public:
    ChestEntity(float x, float y, int type) :
        Entity(x, y, Entity::CHEST), PropertyInventory(CHEST_INV_SIZE, 0) {
        this->set_sprite(type);
        this->solid = true;
        this->interactable = true;
    }
    ChestEntity(float x, float y, int type, std::string& _loot) :
        Entity(x, y, Entity::CHEST), PropertyInventory(CHEST_INV_SIZE, 0) {
        this->set_sprite(type);
        this->solid = true;
        this->interactable = true;

        this->make_loot(_loot);
    }
    ~ChestEntity() { }

    std::string get_interact_name() const;
private:
    void make_loot(std::string& loot);
};

#endif // _CHEST_H
