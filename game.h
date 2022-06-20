#ifndef _GAME_H
#define _GAME_H

#include <string>
#include <unordered_map>
#include "player.h"
#include "map/map.h"

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

    // access functions
    Map* get_current_map();
    Player& get_player();
private:
    // internal action stuff
    bool handle_entity_collide(Entity* entity);

    // references and render info
    sf::Font& font;
    sf::Text text;
    bool draw_label;

    // game data
    std::unordered_map<std::string, Map*> maps;
    Map* current_map;
    Player player;
    int xoff, yoff;
};

#endif // _GAME_G
