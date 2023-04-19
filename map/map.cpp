#include "map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include "../util.h"
#include "../entity/item.h"
#include "../entity/chest.h"
#include "../entity/door.h"
#include "../entity/switch.h"
#include "../entity/trap.h"

Map::Map(std::string path) {
    // open .map file
    std::ifstream mapfile(path);
    if (!mapfile.is_open()) {
        std::cout << "ERROR: unable to open map file: " << path << std::endl;
        std::cerr << "Error: " << std::strerror(errno);
        exit(1031);
    }

    // initialize stuff
    this->w = 0;
    this->h = 0;
    this->sx = -1;
    this->sy = -1;
    this->name.clear();
    this->entities.clear();
    this->portals.clear();
    this->dialogue.clear();

    // fetch a line
    for(std::string line; getline( mapfile, line ); ) {
        // tokenize line
        std::vector<std::string> tokens = split_by_char(line, ';');

        // make sure we got what we expected
        // TODO: make our bounds checking smarter
        if (tokens.size() != 3 && tokens.size() != 2) {
            std::cout << "MALFORMED TOKEN IN FILE: " << tokens[0] << std::endl;
            exit(1032);
        }

        // handle tokens
        if (tokens[0] == "NAME") {
            this->name = tokens[1];
        } else if (tokens[0] == "DIM") {
            std::vector<std::string> dimtokens = split_by_char(tokens[1], ',');

            if (dimtokens.size() == 2) {
                // set size of map
                this->w = std::stoi(dimtokens[0]);
                this->h = std::stoi(dimtokens[1]);

                // initialize fields
                this->water = new uint16_t[this->w * this->h];
                this->ground = new uint16_t[this->w * this->h];
                this->border = new uint16_t[this->w * this->h];
                this->decor = new uint16_t[this->w * this->h];

                // TODO: maybe memset this stuff??
            } else {
                std::cout << "MALFORMED MAPDIM TOKEN IN FILE: " << path << std::endl;
                exit(1033);
            }
        } else if (tokens[0] == "GROUND") {
            this->setup_layer(ground, tokens[1]);
        } else if (tokens[0] == "WATER") {
            this->setup_layer(water, tokens[1]);
        } else if (tokens[0] == "DECORATION") {
            this->setup_layer(decor, tokens[1]);
            this->parse_decor();
        } else if (tokens[0] == "BORDER") {
            this->setup_layer(border, tokens[1]);
        } else if (tokens[0] == "START") {
            this->setup_start(tokens[1]);
        } else if (tokens[0] == "PORTAL") {
            this->setup_portal(tokens[2], tokens[1]);
        } else if (tokens[0] == "CHEST") {
            this->setup_chest(tokens[2], tokens[1]);
        } else if (tokens[0] == "DOOR") {
            this->setup_door(tokens[2], tokens[1]);
        } else if (tokens[0] == "SWITCH") {
            this->setup_switch(tokens[2], tokens[1]);
        } else if (tokens[0] == "TRAP") {
            this->setup_trap(tokens[2], tokens[1]);
        }
    }

    this->build_tile_vert_array();
}

int Map::get_width() {
    return this->w;
}

int Map::get_height() {
    return this->h;
}

std::string& Map::get_name() {
    return this->name;
}

int Map::get_start_x() {
    return this->sx;
}

int Map::get_start_y() {
    return this->sy;
}

bool check_tile_type_collideable(uint16_t x) {
    if (x == 11 || x == 12 || x == 13 || x == 14) {
        return true;
    }

    if (x == 24 || x == 25 || x == 26 || x == 48 || x == 50 || x == 72 || x == 73 || x == 74) {
        return true;
    }

    return false;
}
bool Map::is_collideable(int x, int y) {
    if (x >= w || x < 0 || y >= h || y < 0) {
        return true;
    }

    int water = this->water[y * w + x];
    int ground = this->ground[y * w + x];
    int border = this->border[y * w + x];

    if (check_tile_type_collideable(border)){
        return true;
    }

    if (ground != 65535) {
        return check_tile_type_collideable(ground);
    } else {
        return check_tile_type_collideable(water);
    }
}

bool Map::get_portal(int x, int y, Portal** portal) {
    for (Portal* p : portals) {
        if ((p->get_x() <= x) && (p->get_y() <= y) &&
            (p->get_x() + p->get_w() > x) &&
            (p->get_y() + p->get_h() > y)) {
            (*portal) = p;
            return true;
        }
    }

    return false;
}

void Map::update(float dt) {
    for (Entity* e : this->entities) {
        e->animate(dt);
    }
}

int Map::get_n_entities() {
    return this->entities.size();
}

Entity* Map::get_entity_at(int idx) {
    return this->entities[idx];
}

void Map::add_entity(Entity* entity) {
    this->entities.push_back(entity);
    std::cout << "num=" << entities.size() << std::endl;
}

void Map::remove_entity(int idx) {
    delete this->entities[idx];
    this->entities[idx] = nullptr;
}

void Map::flush_null() {
    this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), nullptr), this->entities.end());
}

std::unordered_map<std::string, Entity*>& Map::get_named_entities() {
    return this->named_entities;
}

std::pair<std::multimap<std::string, Entity*>::iterator, std::multimap<std::string, Entity*>::iterator> Map::get_triggers_for_entity(std::string& name) {
    return trigger_table.equal_range(name);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

    // draw the vertex array
    target.draw(&tile_vertices[0], tile_vertices.size(), sf::Quads, states);

    // iterate and draw each entity
    for (Entity* e : this->entities) {
        target.draw((*e), states);
    }
}

void Map::setup_layer(uint16_t* layer, std::string& input) {
    // split the input string into a useable list
    std::vector<std::string> values = split_by_char(input, ',');

    // check that the result is the right size
    if (values.size() != this->w * this->h) {
        std::cout << "LAYER ENTRY HAS INCORRECT LENGTH!" << std::endl;
        exit(1034);
    } else {
        // copy data into the correct layer, replacing zero with -1
        for (size_t i = 0; i < values.size(); i++) {
            uint16_t d = (uint16_t) std::stoi(values[i]);
            layer[i] = d - 1;
        }
    }
}

void Map::setup_start(std::string& input) {
    std::vector<std::string> values = split_by_char(input, ',');

    if (values.size() != 2) {
        std::cout << "ERROR: Got invalid START marker in map!" << std::endl;
        exit(1029);
    }

    sx = std::stoi(values[0]);
    sy = std::stoi(values[1]);
}

void Map::setup_portal(std::string& input, std::string& name) {
    std::vector<std::string> values = split_by_char(input, ',');

    if (values.size() != 7) {
        std::cout << "ERROR: Got invalid PORTAL token in map!" << std::endl;
        std::cout << input << std::endl;
        exit(1030);
    }

    Portal* p = new Portal(values[0],
                           std::stoi(values[3]) / 16, std::stoi(values[4]) / 16,
                           std::stoi(values[5]) / 16, std::stoi(values[6]) / 16,
                           std::stoi(values[1]), std::stoi(values[2]));

    this->portals.push_back(p);

    // TODO: handle named portals???
}

void Map::setup_chest(std::string& input, std::string& name) {
    std::vector<std::string> values = split_by_char(input, ',');

    if (values.size() != 4) {
        std::cout << "ERROR: Got invalid CHEST token in map!" << std::endl;
        exit(999);
    }

    // decode position
    float x = std::stof(values[0]);
    float y = std::stof(values[1]) - 1;
    int type = std::stoi(values[2]);

    ChestEntity* ce = new ChestEntity(x, y, type, values[3]);
    this->entities.push_back(ce);

    // check entity name
    if (name != "_") {
        this->named_entities[name] = ce;
    }
}

void Map::setup_door(std::string& input, std::string& name) {
    std::vector<std::string> values = split_by_char(input, ',');

    if (values.size() != 6) {
        std::cout << "ERROR: Got invalid DOOR token in map!" << std::endl;
        exit(993);
    }

    // decode position
    float x = std::stof(values[0]);
    float y = std::stof(values[1]) - 1;
    int type = std::stoi(values[2]);
    int alt = std::stoi(values[3]);
    bool toggleable = std::stoi(values[5]) > 0;

    // figure out trigger
    std::vector<std::string> trigs = split_by_char(values[4], ':');
    DoorEntity::TriggerType trig_type = DoorEntity::TriggerType::NONE;
    std::string trigger;
    if (trigs.size() == 2) {
        if (trigs[0] == "switch") {
            trig_type = DoorEntity::TriggerType::SWITCH;
            trigger = trigs[1];
        } else if (trigs[0] == "key") {
            trig_type = DoorEntity::TriggerType::ITEM;
            trigger = trigs[1];
        } else {
            std::cout << "Warning: Unexpected trigger value for door, defaulting to NONE." << std::endl;
            trig_type = DoorEntity::TriggerType::NONE;
        }
    } else {
        trig_type = DoorEntity::TriggerType::NONE;
    }

    // instantiate door and save it
    DoorEntity* de = new DoorEntity(x, y, type, alt, trig_type, trigger, toggleable);
    this->entities.push_back(de);

    // check entity name
    if (name != "_") {
        this->named_entities[name] = de;
    }
}

void Map::setup_switch(std::string& input, std::string& name) {
    std::vector<std::string> values = split_by_char(input, ',');

    if (values.size() != 6) {
        std::cout << "ERROR: Got invalid DOOR token in map!" << std::endl;
        exit(993);
    }

    // decode params
    float x = std::stof(values[0]);
    float y = std::stof(values[1]) - 1;
    int type = std::stoi(values[2]);
    int alt = std::stoi(values[3]);
    bool rq_prev = values[4] != "_";
    int reset_counter = std::stoi(values[5]);

    // instantiate switch and save it
    SwitchEntity* se = new SwitchEntity(x, y, type, alt, rq_prev, values[4], reset_counter);
    this->entities.push_back(se);

    // check entity name
    if (name != "_") {
        this->named_entities[name] = se;
    }
}

void Map::setup_trap(std::string& input, std::string& name) {
    std::vector<std::string> values = split_by_char(input, ',');

    if (values.size() != 9) {
        std::cout << "ERROR: Got invalid DOOR token in map!" << std::endl;
        exit(993);
    }

    // decode params
    float x = std::stof(values[0]);
    float y = std::stof(values[1]) - 1;
    int type = std::stoi(values[2]);
    int alt = std::stoi(values[3]);
    int dmg = std::stoi(values[4]);
    int dx = std::stoi(values[5]);
    int dy = std::stoi(values[6]);

    bool has_arm = (values[7] != "_");
    bool has_disarm = (values[8] != "_");

    // instantiate trap and save it
    TrapEntity* te = new TrapEntity(x, y, type, alt, dmg, dx, dy, has_arm, values[7], has_disarm, values[8]);
    this->entities.push_back(te);

    // check entity name
    if (name != "_") {
        this->named_entities[name] = te;
    }
}

void Map::parse_decor() {
    for (int i = 0; i < this->w; i++) {
        for (int j = 0; j < this->h; j++) {
            uint16_t t = this->decor[i + (j * w)];
            ItemEntity* ie = ItemData::inst().make_item_from_sprite(t);
            if (ie != nullptr) {
                this->decor[i + (j * w)] = 0;
                ie->set_x(i);
                ie->set_y(j);
                this->entities.push_back(ie);
            }
        }
    }
}

void Map::build_tile_vert_array() {
    tile_vertices.clear();

    build_va_layer(tile_vertices, water);
    build_va_layer(tile_vertices, ground);
    build_va_layer(tile_vertices, border);
    build_va_layer(tile_vertices, decor);
}

void Map::build_va_layer(std::vector<sf::Vertex>& vs, uint16_t* layer) {
    for (int i = 0; i < this->w; i++) {
        for (int j = 0; j < this->h; j++) {
            int t = layer[j * w + i];
            float u = (t % SHEET_WIDTH) * TILE_SIZE;
            float v = (t / SHEET_WIDTH) * TILE_SIZE;
            float x = i * TILE_RENDER_SIZE;
            float y = j * TILE_RENDER_SIZE;
            if (t < MAX_TEXTURE_ID) {
                vs.push_back(sf::Vertex(sf::Vector2(x, y), sf::Vector2(u, v)));
                vs.push_back(sf::Vertex(sf::Vector2(x + TILE_RENDER_SIZE, y), sf::Vector2(u + TILE_SIZE, v)));
                vs.push_back(sf::Vertex(sf::Vector2(x + TILE_RENDER_SIZE, y + TILE_RENDER_SIZE), sf::Vector2(u + TILE_SIZE, v + TILE_SIZE)));
                vs.push_back(sf::Vertex(sf::Vector2(x, y + TILE_RENDER_SIZE), sf::Vector2(u, v + TILE_SIZE)));
            }
        }
    }
}
