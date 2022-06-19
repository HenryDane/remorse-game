#ifndef _MAP_H
#define _MAP_H

/*
	A class which encapsulates a single map.
*/

#include <vector>
#include <string>
#include <mutex>
#include <SFML/Graphics.hpp>
#include "entity.h"
#include "portal.h"
#include "dialogue.h"
#include "config.h"

class Map : public sf::Drawable, public sf::Transformable {
public:
	Map(std::string path);
	Map( const Map& ) = delete;
    Map& operator=( const Map& ) = delete;

    // metadata information
    int get_width();
    int get_height();
    std::string& get_name();

    // spawn information
    int get_start_x();
    int get_start_y();

    // tile functions
    void set_water(int x, int y, int tile);
    void set_ground(int x, int y, int decor);
    void set_border(int x, int y, int decor);
    void set_decor(int x, int y, int decor);
    void get_water(int x, int y);
    void get_ground(int x, int y);
    void get_border(int x, int y);
    void get_decor(int x, int y);

    // play information
    Entity* get_entity_overlap(int x, int y);
    bool is_collideable(int x, int y);
    bool get_portal(int x, int y, Portal** portal);
    bool get_dialogue(int x, int y, Dialogue** dialogue);

    // handle a tick
    void update(float dt);

    // entity functionality
    int get_n_entities();
    Entity* get_entity_at(int idx);
    void add_entity(Entity* entity);
    void remove_entity(int idx);
    void flush_null();

    // draw
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    // utility functions
    void setup_layer(uint16_t* layer, std::string& input);
    void setup_start(std::string& input);
    void setup_portal(std::string& input);
    void build_tile_vert_array();
    void build_va_layer(std::vector<sf::Vertex>& v, uint16_t* layer);

    // non-tile data
	std::vector<Entity*> entities;
	std::vector<Portal*> portals;
	std::vector<Dialogue*> dialogue;

	// metadata
	std::string name;
	int w, h, sx, sy;

	// tile data
	uint16_t* decor;
	uint16_t* border;
	uint16_t* ground;
	uint16_t* water;

	// rendering tiles
    std::vector<sf::Vertex> tile_vertices;
};

#endif
