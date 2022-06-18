#include "invrenderer.h"
#include <iostream>
#include "util.h"

void InvRenderer::show() {
    active = true;
}

void InvRenderer::hide() {
    active = false;

    if (dragged.get_type() != ItemType::NONE) {
        drop_item(dragged);
    }
}

void InvRenderer::toggle() {
    if (active) {
        hide();
    } else {
        show();
    }
}

void InvRenderer::set_map(Map* current_map) {
    this->current_map = current_map;
}

void InvRenderer::on_mouse_click(sf::RenderWindow& window, float x, float y) {
    if (!active) {
        return;
    }

    // switch to world space
    sf::Vector2 mouse = window.mapPixelToCoords(sf::Vector2i(x, y), window.getView());

    // if we are already dragging something and we click outside inventory
    if (dragged.get_type() != ItemType::NONE &&
        !intersect_rect(mouse.x, mouse.y, 100, 150, 600, 300)) {
        drop_item(dragged);
        return;
    }

    // prepare index
    int idx = -1;

    if (intersect_rect(mouse.x, mouse.y, 116, 215, 32, 32) &&
        ((dragged.get_type() == ItemType::ARMOR) ||
          dragged.get_type() == ItemType::NONE)) {
        idx = 0;
    } else if (intersect_rect(mouse.x, mouse.y, 116, 280, 32, 32) &&
        ((dragged.get_type() == ItemType::ARMOR) ||
          dragged.get_type() == ItemType::NONE))  {
        idx = 1;
    } else if (intersect_rect(mouse.x, mouse.y, 200, 215, 32, 32) &&
        ((dragged.get_type() == ItemType::WEAPON) ||
          dragged.get_type() == ItemType::NONE))  {
        idx = 2;
    } else if (intersect_rect(mouse.x, mouse.y, 200, 280, 32, 32) &&
        ((dragged.get_type() == ItemType::WEAPON) ||
          dragged.get_type() == ItemType::NONE))  {
        idx = 3;
    } else {
        // need to calculate from inventory
        // subtract inset position
        mouse.x -= 275.0;
        mouse.y -= 175.0;

        // make sure both numbers make sense
        if (mouse.x < 0 || mouse.y < 0) {
            std::cout << "clicked outside of inset" << std::endl;
            return;
        }

        // calculate index
        idx = ((int) (mouse.x / 50)) + 8 * ((int) (mouse.y / 50)) + 4;
        std::cout << "index=" << idx << std::endl;
        if (idx >= player.get_num_items()) {
            std::cout << "index too large" << std::endl;
            return;
        }
    }

    if (idx < 0) {
        std::cout << "failed to find index" << std::endl;
        return;
    }

    // save position
    itemx = mouse.x + 275;
    itemy = mouse.y + 175;

    // check if items are stackable
    if (player.get_item_at(idx, found).is_stackable(dragged)) {
        Item& item = player.get_item_at(idx, found);
        item.set_count(item.get_count() + dragged.get_count());
        dragged.make_invalid();
    } else {
        // swap the items
        Item temp(dragged);
        dragged = player.get_item_at(idx, found);
        player.get_item_at(idx, found) = temp;
    }
}

void InvRenderer::on_mouse_move(sf::RenderWindow& window, float x, float y) {
    if (!active) {
        return;
    }

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2f world_pos = window.mapPixelToCoords(mouse_pos);
    itemx = world_pos.x;
    itemy = world_pos.y;
}

void InvRenderer::draw(sf::RenderTarget& target) {
    if (!active) {
        return;
    }

    rect.setPosition(100, 150);
    rect.setSize(sf::Vector2f(600, 300));
    target.draw(rect);

    rect.setPosition(275, 175);
    rect.setSize(sf::Vector2f(400, 250));
    target.draw(rect);

    rect.setPosition(275, 175);
    rect.setSize(sf::Vector2f(150, 50));
    target.draw(rect);

    rect.setPosition(272, 172);
    rect.setSize(sf::Vector2f(151, 51));
    rect.setOutlineColor(sf::Color::Black);
    target.draw(rect);
    rect.setOutlineColor(sf::Color::White);

    draw_text(target, "Inventory", 16, 116, 166);

    // draw head slot
    draw_text(target, "HEAD", 12, 116, 200);
    draw_item(target, player.get_item_at(0, found), 116, 215);

    // draw chest slot
    draw_text(target, "CHEST", 12, 116, 265);
    draw_item(target, player.get_item_at(1, found), 116, 280);

    // draw left hand slot
    draw_text(target, "LEFT", 12, 200, 200);
    draw_item(target, player.get_item_at(2, found), 200, 215);

    // draw right hand slot
    draw_text(target, "RIGHT", 12, 200, 265);
    draw_item(target, player.get_item_at(3, found), 200, 280);

    // draw items
    for (int i = 0; i < 40 && i + 4 < player.get_num_items(); i++) {
        draw_item(target,
                  player.get_item_at(i + 4, found),
                  285 + (i % 8) * 50,
                  185 + (i / 8) * 50);
    }

    if (dragged.get_type() != ItemType::NONE) {
        draw_item(target, dragged, itemx, itemy);
    }
}

void InvRenderer::draw_text(sf::RenderTarget& target, std::string str, float size, float x, float y) {
    text.setString(str);
    text.setCharacterSize(size);
    text.setPosition(x, y);
    target.draw(text);
}

void InvRenderer::draw_item(sf::RenderTarget& target, Item& item, float x, float y) {
    int t = ItemData::inst().get_sprite(item);
    float u = (t % SHEET_WIDTH) * TILE_SIZE;
    float v = (t / SHEET_WIDTH) * TILE_SIZE;
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(u, v, TILE_SIZE, TILE_SIZE));
    target.draw(sprite);
    sprintf(tmp, "%d", item.get_count());
    text.setString(tmp);
    text.setCharacterSize(16);
    sf::FloatRect fr = text.getLocalBounds();
    text.setPosition(x + 32 - fr.width, y + 30 - fr.height);
    target.draw(text);
}

void InvRenderer::drop_item(Item& item) {
    ItemEntity* ie = new ItemEntity(player.get_x(), player.get_y(), item);
    if (current_map != nullptr) {
        current_map->add_entity(ie);
    }
    item.make_invalid();
}
