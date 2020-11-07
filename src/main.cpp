#include <iostream>

#include "Options.hpp"
#include "Map.hpp"
#ifdef VISUAL
#include "Drawer.hpp"
#endif
#include "Evolution.hpp"

int main(int argc, char* argv[]) {
	Options::configure(argc, argv);

	Map::loadMap();
	Map::showMap();

	#ifdef VISUAL
	Drawer::init(Map::points);
	#endif

	Evolution::start();

	#ifdef VISUAL
	Drawer::draw();
	Drawer::wait();
	#endif

	return 0;
}