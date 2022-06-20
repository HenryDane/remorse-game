#include "hud.h"

void HUD::draw(sf::RenderTarget& target, Player& player) {
    // draw the background
    target.draw(rect);

    // draw the items
    bool found;

    text.setCharacterSize(16);
    text.setPosition(240, 8);
    sprintf(tmp, "HP: %03.0f / 100", player.get_health());
    text.setString(tmp);
    target.draw(text);

    text.setString("Time Remaining");
    text.setPosition(480, 8);
    target.draw(text);
    text.setString("8:14.000");
    text.setPosition(496, 32);
    text.setFillColor(sf::Color::Green);
    target.draw(text);
    text.setFillColor(sf::Color::White);

    text.setCharacterSize(8);

    draw_item(target, player.get_item_at(0, found), 48, 8);
    text.setString("Helm");
    text.setPosition(16, 20);
    target.draw(text);
    draw_item(target, player.get_item_at(1, found), 48, 48);
    text.setString("Chest");
    text.setPosition(16, 60);
    target.draw(text);
    draw_item(target, player.get_item_at(2, found), 128, 8);
    text.setString("Left");
    text.setPosition(96, 20);
    target.draw(text);
    draw_item(target, player.get_item_at(3, found), 128, 48);
    text.setString("Right");
    text.setPosition(96, 60);
    target.draw(text);

    text.setString("Quick Access");
    text.setPosition(240, 32);
    target.draw(text);
    for (int i = 4; i < 7; i++) {
        draw_item(target, player.get_item_at(i, found), 240 + (i - 4) * 48, 48);
    }
}

void HUD::draw_item(sf::RenderTarget& target, Item& item, float x, float y) {
    int t = ItemData::inst().get_sprite(item);
    float u = (t % SHEET_WIDTH) * TILE_SIZE;
    float v = (t / SHEET_WIDTH) * TILE_SIZE;
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(u, v, TILE_SIZE, TILE_SIZE));
    target.draw(sprite);

    sprintf(tmp, "%d", item.get_count());
    text.setString(tmp);
    text.setCharacterSize(8);
    sf::FloatRect fr = text.getLocalBounds();
    text.setPosition(x + 32 - fr.width, y + 30 - fr.height);
    target.draw(text);
}
