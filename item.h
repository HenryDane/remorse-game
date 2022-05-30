#ifndef _ITEM_H
#define _ITEM_H

#include "entity.h"

class Item {
public:
    enum Type {
        NONE = 129,
        // generic items
        BERRIES = 398, APPLE = 372, TOMATO = 345,
        BOTTLE = 103, HALF_MILK = 14,MILK = 105, PLANT = 106
        LOG = 61, STICK = 60, ROCK = 59
        WHEAT_SEEDS = 316, TOMATO_SEEDS = 340
    };

    Item(Type t);
    Item(Type t, int n);
    virtual ~Item();

    Type get_type();
    int get_count();
    void set_count(int n);
private:
    Type t;
    int n;
};

class ItemEntity : public Entity {
public:
    ItemEntity(int x, int y, Item item) : Entity(x, y, Entity::ITEM), item(item) {
        this->set_sprite(item.get_type());
    };
    ItemEntity(int x, int y, Item::Type type) : Entity(x, y, Entity::ITEM), item(type, 1) {
        this->set_sprite(item.get_type());
    };
    ~ItemEntity();

    // item specific
    Item& get_item();

    // override
    void animate(float dt);
private:
    float time;
    Item item;
};

#endif
