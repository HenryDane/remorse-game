#ifndef _ENTITY_H
#define _ENTITY_H

/*
	Base entity class. Should be inherited from to implement other entities

	TODO: movement state animation handler
	TODO: additional handling for
*/

#include <SFML/Graphics.hpp>

#define ENTITY_DEFAULT_SPRITE 151

class Entity : public sf::Drawable, public sf::Transformable {
public:
	enum Type {
		NONE, PLAYER, ITEM
	};
	Entity();
	Entity(float x, float y, Type t);
	virtual ~Entity();

	// position
	float get_x();
	float get_y();
	void set_x(float x);
	void set_y(float y);

	// type info
	virtual Type get_type();

	// texture info
	int get_sprite();
	void set_sprite(int sprite);

	// handle updates
	virtual void animate(float dt);

	// draw the object
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
    // get/set the offset for the sprite
    void set_render_offset_x(float x);
    void set_render_offset_y(float y);
    float get_render_offset_x();
    float get_render_offset_y();
private:
    void build_vert_array();

    sf::Vertex vertices[4];
    int sprite = ENTITY_DEFAULT_SPRITE;
	float x, y, rx, ry;
	Type t;
};

#endif
