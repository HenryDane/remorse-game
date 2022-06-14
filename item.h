#ifndef _ITEM_H
#define _ITEM_H

#include "entity.h"

#include <string>
#include <unordered_map>

#define INVALID_ITEM_NAME "__invalid__"

struct item_def_t {
    std::string name;
    int sprite;

    // properties
    int hp, atk, def, spd;
};

class Item;

class ItemData {
public:
    // item type enum
    enum Type {
        NONE, ITEM, CONSUMABLE, EQUIPPABLE, WEAPON
    };

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

    // get instance
    static ItemData& inst();
private:
    std::unordered_map<std::string, item_def_t> item_defs;
};

class Item {
public:
    Item();
    virtual ~Item();

    ItemData::Type get_type();
    int get_count();
    void set_count(int n);
    std::string get_name();

    bool is_equal(Item& item) const;
    void make_invalid();
private:
    friend class ItemData;

    // only itemdata can instantiate this
    Item(ItemData::Type t, int n, std::string name);

    // fields
    std::string name;
    ItemData::Type t;
    int n;
};

class ItemEntity : public Entity {
public:
    ItemEntity(int x, int y, Item item) : Entity(x, y, Entity::ITEM), item(item) {
//        this->set_sprite(item.get_type());
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
