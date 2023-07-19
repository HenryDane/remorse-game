#include "chestinvrenderer.h"
#include <iostream>
#include "../util.h"

void ChestInvRenderer::show() {
    active = true;
}

void ChestInvRenderer::hide() {
    active = false;

    if (dragged.get_type() != ItemType::NONE) {
        drop_item(dragged);
    }
}

void ChestInvRenderer::toggle() {
    if (active) {
        hide();
    } else {
        show();
    }
}

void ChestInvRenderer::set_map(Map* current_map) {
    this->current_map = current_map;
}

void ChestInvRenderer::set_chest(ChestEntity* current_chest) {
    this->current_chest = current_chest;
}

void ChestInvRenderer::on_mouse_click(sf::RenderWindow& window, float x, float y) {
    if (!active) {
        return;
    }

    // switch to world space
    sf::Vector2 mouse = window.mapPixelToCoords(sf::Vector2i(x, y), window.getView());

    // if we are already dragging something and we click outside inventory
    if (dragged.get_type() != ItemType::NONE &&
        !intersect_rect(mouse.x, mouse.y, 50, 150, 750, 550)) {
        drop_item(dragged);
        return;
    }

    // prepare state
    int idx = -1;
    bool is_chest = mouse.x < 375.0f;

    if (is_chest && current_chest == nullptr) {
        return;
    }

    if (!is_chest) {
        // subtract inset position
        mouse.x -= 425.0;
        mouse.y -= 200.0;

        // make sure both numbers make sense
        if (mouse.x < 0 || mouse.y < 0 ||
            mouse.x > 300 || mouse.y > 325) {
            std::cout << "clicked outside of inset" << std::endl;
            return;
        }

        // calculate index
        idx = ((int) (mouse.x / 50)) + 6 * ((int) (mouse.y / 50)) + 4;
        std::cout << "index=" << idx << std::endl;
        if (idx >= player.get_num_items()) {
            std::cout << "index too large" << std::endl;
            return;
        }
    } else {
        // subtract inset position
        mouse.x -= 75.0;
        mouse.y -= 200.0;

        // make sure both numbers make sense
        if (mouse.x < 0 || mouse.y < 0 ||
            mouse.x > 300 || mouse.y > 325) {
            std::cout << "clicked outside of inset" << std::endl;
            return;
        }

        // calculate index
        idx = ((int) (mouse.x / 50)) + 6 * ((int) (mouse.y / 50));
        std::cout << "index=" << idx << std::endl;
        if (idx >= current_chest->get_num_items()) {
            std::cout << "index too large" << std::endl;
            return;
        }
    }

    if (idx < 0) {
        std::cout << "failed to find index" << std::endl;
        return;
    }

    // save position
    itemx = mouse.x + (is_chest ? 75.0f : 425.0f);
    itemy = mouse.y + 200.0f;

    if (!is_chest) {
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
    } else {
        // check if items are stackable
        if (current_chest->get_item_at(idx, found).is_stackable(dragged)) {
            Item& item = current_chest->get_item_at(idx, found);
            item.set_count(item.get_count() + dragged.get_count());
            dragged.make_invalid();
        } else {
            // swap the items
            Item temp(dragged);
            dragged = current_chest->get_item_at(idx, found);
            current_chest->get_item_at(idx, found) = temp;
        }
    }
}

void ChestInvRenderer::on_mouse_move(sf::RenderWindow& window, float x, float y) {
    if (!active) {
        return;
    }

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2f world_pos = window.mapPixelToCoords(mouse_pos);
    itemx = world_pos.x;
    itemy = world_pos.y;
}

void ChestInvRenderer::draw(sf::RenderTarget& target) {
    if (!active) {
        return;
    }

    // background box
    rect.setPosition(50, 150);
    rect.setSize(sf::Vector2f(700, 400));
    target.draw(rect);

    // chest box
    rect.setPosition(75, 200);
    rect.setSize(sf::Vector2f(300, 325));
    target.draw(rect);

    // player box
    rect.setPosition(425, 200);
    rect.setSize(sf::Vector2f(300, 325));
    target.draw(rect);

    // labels
    draw_text(target, "Chest Inventory", 16, 75, 166);
    draw_text(target, "Player Inventory", 16, 425, 166);

    // draw chest items
    if (current_chest != nullptr) {
        for (int i = 0; i < 40 && i < current_chest->get_num_items(); i++) {
            draw_item(target,
                      current_chest->get_item_at(i, found),
                      84 + (i % 6) * 50,
                      212 + (i / 6) * 50);
        }
    }

    // draw player items
    for (int i = 0; i < 40 && i + 4 < player.get_num_items(); i++) {
        draw_item(target,
                  player.get_item_at(i + 4, found),
                  434 + (i % 6) * 50,
                  212 + (i / 6) * 50);
    }

    if (dragged.get_type() != ItemType::NONE) {
        draw_item(target, dragged, itemx, itemy);
    }
}

void ChestInvRenderer::draw_text(sf::RenderTarget& target, std::string str, float size, float x, float y) {
    text.setString(str);
    text.setCharacterSize(size);
    text.setPosition(x, y);
    target.draw(text);
}

void ChestInvRenderer::draw_item(sf::RenderTarget& target, Item& item, float x, float y) {
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

void ChestInvRenderer::drop_item(Item& item) {
    ItemEntity* ie = new ItemEntity(player.get_x(), player.get_y(), item);
    if (current_map != nullptr) {
        current_map->add_entity(ie);
    }
    item.make_invalid();
}
