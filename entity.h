#ifndef _ENTITY_H
#define _ENTITY_H

/*
	Base entity class. Should be inherited from to implement other entities

	TODO: movement state animation handler
	TODO: additional handling for
*/

#include "structures.h"

class Entity {
public:
	enum Type {
		NONE, PLAYER
	};
	Entity();
	Entity(int x, int y);
	virtual ~Entity();

	// position
	int get_x();
	int get_y();
	int set_x();
	int set_y();

	// type info
	virtual Type get_type();

	// texture info
	virtual int get_sprite();

	// handle updates
	virtual void update(float dt);
private:
	int x, y;
};

#endif
