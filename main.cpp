#include <SFML/Graphics.hpp>
#include <iostream>
#include "map.h"
#include "player.h"
#include "item.h"
#include "util.h"
#include "config.h"

void move_player(Map* current_map, Player& player, int dx, int dy) {
    std::cout << "facing=" << player.get_facing() << " dx=" << dx << " dy=" << dy << std::endl;
    if (!(player.get_facing() == Player::DOWN  && dx ==  0 && dy ==  1) &&
        !(player.get_facing() == Player::UP    && dx ==  0 && dy == -1) &&
        !(player.get_facing() == Player::RIGHT && dx ==  1 && dy ==  0) &&
        !(player.get_facing() == Player::LEFT  && dx == -1 && dy ==  0)) {
        std::cout << "facing wrong!" << std::endl;
        player.set_facing(dx, dy);
        return;
    }

    if (!current_map->is_collideable(player.get_x() + dx, player.get_y() + dy)) {
        player.set_dxdy(dx, dy);
    }
}

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

    // setup reusable sprite
    sf::Sprite sprite;
    sprite.setTexture(spritesheet);

    // get the player ready
    Player player(13, 10, 100.0f);
    ItemEntity* test_item = new ItemEntity(10, 10, Item(Item::MILK));

    // load a temporary map
    Map* current_map = new Map("result.map");
    current_map->add_entity(test_item);

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
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::D) {
                    move_player(current_map, player, 1, 0);
                } else if (event.key.code == sf::Keyboard::A) {
                    move_player(current_map, player, -1, 0);
                } else if (event.key.code == sf::Keyboard::W) {
                    move_player(current_map, player, 0, -1);
                } else if (event.key.code == sf::Keyboard::S) {
                    move_player(current_map, player, 0, 1);
                }
            } else if (event.type == sf::Event::Resized) {
                window.setView(getLetterboxView( window.getView(), event.size.width, event.size.height ));
            }
        }

        // update the map
        current_map->update(0.01667f);

        // draw the map
        current_map->setPosition((10 - player.get_render_x()) * 32, (7 - player.get_render_y()) * 32);
        window.draw(*current_map, render_state);

        // draw the player
        player.animate(0.01667f);
        window.draw(player, render_state);

        // finish the frame
        window.display();
    }

    return 0;
}
