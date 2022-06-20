#include <SFML/Graphics.hpp>
#include <iostream>
#include "map/map.h"
#include "player.h"
#include "util.h"
#include "config.h"
#include "guis/hud.h"
#include "guis/invrenderer.h"
#include "game.h"

int main() {
    // create the window
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(SCREEN_W, SCREEN_H), "2D RPG");
    window.setKeyRepeatEnabled(true);
    window.setFramerateLimit(60);

    // create the rendertexture
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(SCREEN_W * 2, SCREEN_H * 2)) {
        std::cout << "could not create render texture!" << std::endl;
        return 1020;
    }

    // load spritesheet
    sf::Texture spritesheet;
    if (!spritesheet.loadFromFile("asset/spritesheet.png")) {
        std::cout << "Unable to load spritesheet." << std::endl;
        exit(1090);
    }

    // setup reusable sprite
    sf::Sprite sprite;
    sprite.setTexture(spritesheet);

    // setup font
    sf::Font font;
    if (!font.loadFromFile("asset/telegrama_render.otf")) {
        std::cout << "Could not find font: telegrama_raw.ttf" << std::endl;
        exit(0);
    }

    // setup HUD elements
    HUD hud(font, spritesheet);

    // setup item data registry
    ItemData itemdata("asset/items.txt");

    // setup the game
    Game game("asset/startup.txt", font);

    // setup inventory renderer
    InvRenderer invrenderer(font, spritesheet, game.get_player());
    invrenderer.set_map(game.get_current_map());

    // create renderstate
    sf::RenderStates render_state(&spritesheet);

    // track frame time
    sf::Clock clock;

    // main loop
    while (window.isOpen()) {
        // clear the window
        window.clear();

        // get delta time
        sf::Time elapsed = clock.restart();
        float delta_time = elapsed.asSeconds();

        // process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // close the window if we're asked to do so
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::D) {
                    game.move_player(1, 0);
                } else if (event.key.code == sf::Keyboard::A) {
                    game.move_player(-1, 0);
                } else if (event.key.code == sf::Keyboard::W) {
                    game.move_player(0, -1);
                } else if (event.key.code == sf::Keyboard::S) {
                    game.move_player(0, 1);
                } else if (event.key.code == sf::Keyboard::I) {
                    invrenderer.toggle();
                } else if (event.key.code == sf::Keyboard::Escape) {
                    invrenderer.hide();
                }

                else if (event.key.code == sf::Keyboard::Q) {
                    game.get_player().add_item(ItemData::inst().make_item("Plant"));
                } else if (event.key.code == sf::Keyboard::E) {
                    game.get_player().add_item(ItemData::inst().make_item("Golden Armor"));
                } else if (event.key.code == sf::Keyboard::R) {
                    game.get_player().add_item(ItemData::inst().make_item("Wheat Seeds", 10));
                } else if (event.key.code == sf::Keyboard::T) {
                    game.get_player().add_item(ItemData::inst().make_item("Tomato Seeds", 7));
                }
            } else if (event.type == sf::Event::Resized) {
                window.setView(getLetterboxView( window.getView(), event.size.width, event.size.height ));
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    invrenderer.on_mouse_click(window, event.mouseButton.x, event.mouseButton.y);
                }
            } else if (event.type == sf::Event::MouseMoved) {
                invrenderer.on_mouse_move(window, event.mouseMove.x, event.mouseMove.y);
                game.on_mouse_move(window, event.mouseMove.x, event.mouseMove.y);
            }
        }

        // render game
        game.draw(window, render_state, delta_time);

        // draw the hud
        hud.draw(window, game.get_player());

        // draw the inventory
        invrenderer.draw(window);

        // finish the frame
        window.display();
    }

    return 0;
}
