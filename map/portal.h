#ifndef _PORTAL_H
#define _PORTAL_H

#include <string>

class Portal {
public:
	Portal(std::string& name, int x, int y, int w, int h, int tx, int ty);

	std::string& get_name();
	int get_x();
	int get_y();
	int get_w();
	int get_h();
	int get_target_x();
	int get_target_y();
private:
	std::string s;
	int x, y, w, h, tx, ty;
};

#endif
