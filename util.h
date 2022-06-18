#ifndef _UTIL_H
#define _UTIL_H

#include <sstream>
#include <SFML/Graphics.hpp>

sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);

std::vector<std::string> split_by_char(std::string in, char split);

bool intersect_rect(float x, float y, float sx, float sy, float w, float h);
#endif
