#ifndef _ITEM_H
#define _ITEM_H

#include "entity.h"

#include <string>
#include <unordered_map>

#define INVALID_ITEM_NAME "__invalid__"

enum class ItemType {
    NONE, ITEM, CONSUMABLE, ARMOR, WEAPON
};

struct item_def_t {
    std::string name;
    int sprite;
    ItemType type;

    // properties
    int hp, atk, def, spd;
};

class Item;

class ItemData {
public:
    // constructors and destructor
    ItemData(std::string path);
    ItemData(ItemData const&) = delete;
    void operator=(ItemData const&) = delete;
    ~ItemData();

    // create items
    Item make_item(std::string name);
    Item make_item(std::string name, int n);

    // item info
    int get_sprite(Item& item);
    int get_sprite(std::string& name);
    bool is_item_defined(std::string name);

    // get instance
    static ItemData& inst();
private:
    std::unordered_map<std::string, item_def_t> item_defs;
};

class Item {
public:
    Item();
    virtual ~Item();

    ItemType get_type();
    int get_count();
    void set_count(int n);
    std::string get_name();

    bool is_equal(Item& item) const;
    bool is_stackable(Item& item) const;
    void make_invalid();
private:
    friend class ItemData;

    Item(std::string name, int n, ItemType type);

    // fields
    std::string name;
    ItemType t;
    int n;
};

class ItemEntity : public Entity {
public:
    ItemEntity(int x, int y, Item item) : Entity(x, y, Entity::ITEM), item(item) {
        this->set_sprite(ItemData::inst().get_sprite(item));
        this->pickup_remaining = 3.0f;
    };
    ItemEntity(int x, int y, Item item, float pickup_delay) : Entity(x, y, Entity::ITEM), item(item) {
        this->set_sprite(ItemData::inst().get_sprite(item));
        this->pickup_remaining = pickup_delay;
    };
    ~ItemEntity();

    // item specific
    Item& get_item();
    void make_invalid();

    bool is_pickup_ok();

    // override
    void animate(float dt);
private:
    float time, pickup_remaining;
    Item item;
};

#endif
