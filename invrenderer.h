#ifndef _INV_RENDERER_H
#define _INV_RENDERER_H

#include <SFML/Graphics.hpp>
#include "player.h"
#include "map.h"

class InvRenderer {
public:
    InvRenderer(sf::Font& _f, sf::Texture& _t, Player& _p) :
        font(_f), texture(_t), player(_p) {

        rect = sf::RectangleShape();
        rect.setFillColor(sf::Color::Black);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(2);
        rect.setPosition(100, 150);
        rect.setSize(sf::Vector2f(600, 300));

        text = sf::Text("Inventory", font);
        text.setPosition(116, 166);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(16);

        sprite = sf::Sprite(texture);
        sprite.setScale(2.0f, 2.0f);

        active = false;
        itemx = -100;
        itemy = -100;

        current_map = nullptr;
    }
    ~InvRenderer() { };

    void show();
    void hide();
    void toggle();

    void set_map(Map* current_map);

    void draw(sf::RenderTarget& target);
    void on_mouse_click(sf::RenderWindow& window, float x, float y);
    void on_mouse_move(sf::RenderWindow& window, float x, float y);
private:
    // utility functions
    void draw_item(sf::RenderTarget& target, Item& item, float x, float y);
    void draw_text(sf::RenderTarget& target, std::string str, float size, float x, float y);
    void drop_item(Item& item);

    // references to external resources
    sf::Font& font;
    sf::Texture& texture;
    Player& player;
    Map* current_map;

    // reuseable renderables
    sf::RectangleShape rect;
    sf::Text text;
    sf::Sprite sprite;

    // persistent data
    bool found, active;
    char tmp[16];
    Item dragged;
    float itemx, itemy;
};

#endif // _INV_RENDERER_H
