#include "game.h"
#include <iostream>
#include "util.h"

Game::Game(std::string path, sf::Font& _f) : font(_f), player(13, 13, 100.0f) {
    current_map = new Map("untitled.map");
    xoff = 10;
    yoff = 7;

    text = sf::Text("ITEM", font);
    text.setCharacterSize(8);
    text.setFillColor(sf::Color::White);
    text.setPosition(100, 100);

    draw_label = false;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates& state, float dt) {
    // update the map
    current_map->update(dt);

    // draw the map
    current_map->setPosition((xoff - player.get_render_x()) * 32, (yoff - player.get_render_y()) * 32);
    target.draw(*current_map, state);

    // draw the player
    player.animate(dt);
    target.draw(player, state);

    if (draw_label) {
        target.draw(text);
    }
}

void Game::move_player(int dx, int dy) {
    std::cout << "facing=" << player.get_facing() << " dx=" << dx << " dy=" << dy << std::endl;
    if (!(player.get_facing() == Player::DOWN  && dx ==  0 && dy ==  1) &&
        !(player.get_facing() == Player::UP    && dx ==  0 && dy == -1) &&
        !(player.get_facing() == Player::RIGHT && dx ==  1 && dy ==  0) &&
        !(player.get_facing() == Player::LEFT  && dx == -1 && dy ==  0)) {
        std::cout << "facing wrong!" << std::endl;
        player.set_facing(dx, dy);
        return;
    }

    // check collision with entities
    bool has_collided = false;
    for (int i = 0; i < current_map->get_n_entities(); i++) {
        Entity* e = current_map->get_entity_at(i);
        if (e == nullptr) continue;
        std::cout << "entity e=" << e << std::endl;
        if (e->get_x() == player.get_x() + dx &&
            e->get_y() == player.get_y() + dy) {
            if (e->is_solid()) {
                has_collided = true;
            }
            if (handle_entity_collide(e)) {
                current_map->remove_entity(i);
            }
        }
    }
    current_map->flush_null();
    if (has_collided) {
        return;
    }

    if (!current_map->is_collideable(player.get_x() + dx, player.get_y() + dy)) {
        player.set_dxdy(dx, dy);
    }
}

void Game::on_mouse_move(sf::RenderWindow& window, float x, float y) {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2f world_pos = window.mapPixelToCoords(mouse_pos);

    draw_label = false;
    for (int i = 0; i < current_map->get_n_entities(); i++) {
        Entity* e = current_map->get_entity_at(i);
        if (e->get_type() != Entity::ITEM) continue;
        int ex = (e->get_x() - player.get_render_x() + xoff) * 32;
        int ey = (e->get_y() - player.get_render_y() + yoff) * 32;
        if (intersect_rect(world_pos.x, world_pos.y,
                           ex, ey, 32, 32)) {
            text.setString(((ItemEntity*)e)->get_item().get_name());
            sf::FloatRect fr = text.getLocalBounds();
            text.setPosition(sf::Vector2f(ex + (32 - fr.width) / 2.0f,
                                          ey + 32));
            draw_label = true;
            std::cout << "intersected!" << std::endl;
        }
    }
}

Map* Game::get_current_map() {
    return current_map;
}

Player& Game::get_player() {
    return player;
}

bool Game::handle_entity_collide(Entity* entity) {
    if (entity->get_type() == Entity::ITEM) {
        ItemEntity* ie = (ItemEntity*) entity;
        std::cout << "hit item with name: " << ie->get_item().get_name() << std::endl;
        if (!ie->is_pickup_ok()) {
            return false;
        }
        if (player.add_item(ie->get_item())) {
            ie->make_invalid();
            return true;
        }
    }

    return false;
}
