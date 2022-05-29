#include "map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

std::vector<std::string> split_by_char(std::string in, char split) {
    std::stringstream stream(in);
    std::vector<std::string> tokens;
    std::string token;
    while(std::getline(stream, token, split)) {
        tokens.push_back(token);
    }
    return tokens;
}

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
        if (tokens.size() != 2) {
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
        } else if (tokens[0] == "BORDER") {
            this->setup_layer(border, tokens[1]);
        }
    }

    this->build_vert_array();
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

void Map::build_vert_array() {
    vertices.clear();

    build_va_layer(vertices, water);
    build_va_layer(vertices, ground);
    build_va_layer(vertices, border);
    build_va_layer(vertices, decor);
}

void Map::build_va_layer(std::vector<sf::Vertex>& v, uint16_t* layer) {
    for (int i = 0; i < this->w; i++) {
        for (int j = 0; j < this->h; j++) {
            int t = layer[j * w + i];
            float u = (t % SHEET_WIDTH) * 16;
            float v = (t / SHEET_WIDTH) * 16;
            float x = i * 32;
            float y = j * 32;
            if (t < MAX_TEXTURE_ID) {
                vertices.push_back(sf::Vertex(sf::Vector2(x, y), sf::Vector2(u, v)));
                vertices.push_back(sf::Vertex(sf::Vector2(x + 32.0f, y), sf::Vector2(u + 16.0f, v)));
                vertices.push_back(sf::Vertex(sf::Vector2(x + 32.0f, y + 32.0f), sf::Vector2(u + 16.0f, v + 16.0f)));
                vertices.push_back(sf::Vertex(sf::Vector2(x, y + 32.0f), sf::Vector2(u, v + 16.0f)));
            }
        }
    }
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

    // draw the vertex array
    target.draw(&vertices[0], vertices.size(), sf::Quads, states);
}
