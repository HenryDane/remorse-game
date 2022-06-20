#ifndef _PROP_LIVING_H
#define _PROP_LIVING_H

class PropertyLiving {
public:
    PropertyLiving(float health, float max_health) {
        this->health = health;
        this->max_health = max_health;
    }
    ~PropertyLiving() { };

    float get_max_health();
    float get_health();
    void set_health(float health);
    bool change_health(float dh);
private:
    float health, max_health;
};

#endif // _PROP_LIVING_H
