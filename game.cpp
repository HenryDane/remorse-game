#include "game.h"
#include <iostream>
#include <fstream>
#include "util.h"

Game::Game(std::string path, sf::Font& _f) : font(_f), player(13, 13, 100.0f) {
    xoff = 10;
    yoff = 7;

    text = sf::Text("ITEM", font);
    text.setCharacterSize(8);
    text.setFillColor(sf::Color::White);
    text.setPosition(100, 100);

    interact_text = sf::Text("E: __invalid__", font);
    interact_text.setCharacterSize(16);
    interact_text.setFillColor(sf::Color::Black);
    {
    sf::FloatRect r = interact_text.getLocalBounds();
    interact_text.setPosition((SCREEN_W - r.width / 2), 500);
    }

    draw_label = false;

    // parse input file
    std::ifstream startfile(path);
    if (!startfile.is_open()) {
        std::cout << "ERROR: failed to open startfile! " << std::endl;
        exit(1022);
    }

    int line_num = 1;
    for (std::string line; getline(startfile, line); line_num++) {
        std::vector<std::string> tokens = split_by_char(line, ';');

        if (tokens.size() <= 0) {
            continue;
        }

        if (tokens[0] == "MAP") {
            // make sure we have right number of elements!
            if (tokens.size() != 2) {
                std::cout << "ERROR: Malformed map defined on line: " << line_num << std::endl;
                exit(910);
            }

            // load the map
            Map* m = new Map(tokens[1]);
            maps[m->get_name()] = m;
        } else if (tokens[0] == "START") {
            // check for the right number of elements
            if (tokens.size() != 2) {
                std::cout << "ERROR: Malformed starting map entry on line: " << line_num << std::endl;
                exit(911);
            }

            // try to set the map
            if (maps.find(tokens[1]) == maps.end()) {
                std::cout << "ERROR: Requested map does not exist: " << tokens[1] << std::endl;
                exit(912);
            }

            // set the map
            current_map = maps[tokens[1]];
        }
    }
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

    if (draw_interact) {
        target.draw(interact_text);
    }
}

void Game::check_interact() {
    int dx = 0; int dy = 0;

    switch (player.get_facing()) {
    case Player::Facing::DOWN:
        dy = 1;
        break;
    case Player::Facing::LEFT:
        dx = -1;
        break;
    case Player::Facing::RIGHT:
        dx = 1;
        break;
    case Player::Facing::UP:
        dy = -1;
        break;
    }

    draw_interact = false;
    for (int i = 0; i < current_map->get_n_entities(); i++) {
        Entity* e = current_map->get_entity_at(i);
        if (e == nullptr) continue;
        if (e->get_x() == player.get_x() + dx &&
            e->get_y() == player.get_y() + dy) {
            if (e->is_interactable()) {
                interact_text.setString("E: " + e->get_interact_name());
                sf::FloatRect r = interact_text.getLocalBounds();
                interact_text.setPosition((SCREEN_W - r.width) / 2, 500);
                draw_interact = true;
                std::cout << "interact_text=" << interact_text.getString().toAnsiString() << std::endl;
                break;
            }
        }
    }
}

void Game::move_player(int dx, int dy) {
    if (!(player.get_facing() == Player::DOWN  && dx ==  0 && dy ==  1) &&
        !(player.get_facing() == Player::UP    && dx ==  0 && dy == -1) &&
        !(player.get_facing() == Player::RIGHT && dx ==  1 && dy ==  0) &&
        !(player.get_facing() == Player::LEFT  && dx == -1 && dy ==  0)) {
        player.set_facing(dx, dy);
        this->check_interact();
        return;
    }

    // check collision with entities
    bool has_collided = false;
    for (int i = 0; i < current_map->get_n_entities(); i++) {
        Entity* e = current_map->get_entity_at(i);
        if (e == nullptr) continue;
        if (e->get_x() == player.get_x() + dx &&
            e->get_y() == player.get_y() + dy) {
            if (e->is_solid()) {
                has_collided = true;
            }
            CollideResult cres = handle_entity_collide(e);
            if (cres == CollideResult::DELETE) {
                current_map->remove_entity(i);
            }
        }
    }
    current_map->flush_null();
    if (has_collided) {
        this->check_interact();
        return;
    }

    Portal* portal;
    if (current_map->get_portal(player.get_x() + dx,
                                player.get_y() + dy,
                                &portal)) {
        // check if the destination map exists
        if (maps.find(portal->get_name()) == maps.end()) {
            std::cout << "WARNING: Got bad map name from portal: " << portal->get_name() << std::endl;
        } else {
            current_map = maps[portal->get_name()];
            if ((portal->get_target_x() == -1) ||
                (portal->get_target_y() == -1)) {
                player.set_x(current_map->get_start_x());
                player.set_y(current_map->get_start_y());
            } else {
                player.set_x(portal->get_target_x());
                player.set_y(portal->get_target_y());
            }
        }
    }

    if (!current_map->is_collideable(player.get_x() + dx, player.get_y() + dy)) {
        player.set_dxdy(dx, dy);
    }

    //iterate through named entities and check intersection
    for (auto& entry : current_map->get_named_entities()) {
        Entity* entity = entry.second;
        std::string name = entry.first;
        // if overlap
        if (entity->get_x() == player.get_x() + dx &&
            entity->get_y() == player.get_y()) {
            // get all linked entities
            std::pair<std::multimap<std::string, Entity*>::iterator, std::multimap<std::string, Entity*>::iterator> rit =
                current_map->get_triggers_for_entity(name);
            std::multimap<std::string, Entity*>::iterator it;
            for (it = rit.first; it != rit.second; it++) {
                // process each linked entity
                Entity* e = (*it).second;
            }
        }
    }

    this->check_interact();
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
        }
    }
}

void Game::try_interact() {
    int dx = 0; int dy = 0;

    switch (player.get_facing()) {
    case Player::Facing::DOWN:
        dy = 1;
        break;
    case Player::Facing::LEFT:
        dx = -1;
        break;
    case Player::Facing::RIGHT:
        dx = 1;
        break;
    case Player::Facing::UP:
        dy = -1;
        break;
    }

    for (int i = 0; i < current_map->get_n_entities(); i++) {
        Entity* e = current_map->get_entity_at(i);
        if (e == nullptr) continue;
        if (e->get_x() == player.get_x() + dx &&
            e->get_y() == player.get_y() + dy) {

            if (e->get_type() == Entity::Type::CHEST) {
                cinvr->set_chest((ChestEntity*) e);
                cinvr->show();
            }
        }
    }
}

Map* Game::get_current_map() {
    return current_map;
}

Player& Game::get_player() {
    return player;
}

Game::CollideResult Game::handle_entity_collide(Entity* entity) {
    switch (entity->get_type()) {
    case Entity::Type::ITEM:
        {
            ItemEntity* ie = (ItemEntity*) entity;
            if (!ie->is_pickup_ok()) {
                return CollideResult::NONE;
            }
            if (player.add_item(ie->get_item())) {
                ie->make_invalid();
                return CollideResult::DELETE;
            }
        }
        break;
    default:
        break;
    }

    return CollideResult::NONE;
}
