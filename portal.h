#ifndef _PORTAL_H
#define _PORTAL_H

class Portal {
public:
	Portal(std::string& name, int x, int y, int tx, int ty);

	std::string& get_name();
	int get_x();
	int get_y();
	int get_target_x();
	int get_target_y();
private:
	std::string s;
	int x, y, tx, ty;
};

#endif
