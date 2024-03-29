#ifndef _GAME_H
#define _GAME_H

#include <string>
#include <unordered_map>
#include "player.h"
#include "map/map.h"
#include "guis/chestinvrenderer.h"

class Game {
public:
    Game(std::string path, sf::Font& _f);
    ~Game() { };

    // each frame
    void draw(sf::RenderTarget& target, sf::RenderStates& state, float dt);
    void update();

    // input functions
    void move_player(int dx, int dy);
    void on_mouse_move(sf::RenderWindow& window, float x, float y);
    void try_interact();
    void try_hotbar(int idx);

    // access functions
    Map* get_current_map();
    Player& get_player();

    // reference functions
    void set_cinvr(ChestInvRenderer* _cinvr) { this->cinvr = _cinvr; };
private:
    enum CollideResult {
        NONE, DELETE, INTERACT
    };
    // internal action stuff
    CollideResult handle_entity_collide(Entity* entity);
    void check_interact();

    // references and render info
    sf::Font& font;
    sf::Text text;
    sf::Text interact_text;
    bool draw_label, draw_interact;
    ChestInvRenderer* cinvr;

    // game data
    std::unordered_map<std::string, Map*> maps;
    Map* current_map;
    Player player;
    int xoff, yoff;
};

#endif // _GAME_G
