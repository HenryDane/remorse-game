#include <SFML/Graphics.hpp>
#include <iostream>
#include "map.h"
//#include "draw.h"

#define SCREEN_W 800
#define SCREEN_H 600

Map* current_map;

int main() {
    // create the window
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(SCREEN_W, SCREEN_H), "2D RPG");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    // create the rendertexture
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(SCREEN_W * 2, SCREEN_H * 2)) {
        std::cout << "could not create render texture!" << std::endl;
        return 1020;
    }

    // load spritesheet
    sf::Texture spritesheet;
    if (!spritesheet.loadFromFile("asset/sprites.png")) {
        std::cout << "Unable to load spritesheet." << std::endl;
        exit(1090);
    }

    // load a temporary map
    current_map = new Map("result.map");

    // create renderstate
    sf::RenderStates render_state(&spritesheet);

    while (window.isOpen()) {
        // clear the window
        window.clear();

        // process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // close the window if we're asked to do so
                window.close();
            }
        }

        // draw the map
        window.draw(*current_map, render_state);

        // finish the frame
        window.display();
    }

    return 0;
}
