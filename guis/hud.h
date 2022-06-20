#ifndef _HUD_H
#define _HUD_H

#include <SFML/Graphics.hpp>
#include "../player.h"

class HUD {
public:
    HUD(sf::Font& _font, sf::Texture& _texture) : font(_font) {
        rect = sf::RectangleShape();
        rect.setFillColor(sf::Color::Black);
        rect.setPosition(0, 0);
        rect.setSize(sf::Vector2f(800, 100));

        text = sf::Text("HP: 100 / 100", font);
        text.setPosition(16, 16);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(16);

        sprite = sf::Sprite(_texture);
        sprite.setScale(2.0f, 2.0f);
    };
    ~HUD() { };

    void draw(sf::RenderTarget& target, Player& player);
private:
    void draw_item(sf::RenderTarget& target, Item& item, float x, float y);

    sf::RectangleShape rect;
    sf::Text text;
    sf::Sprite sprite;
    sf::Font& font;
    char tmp[256];
};

#endif
