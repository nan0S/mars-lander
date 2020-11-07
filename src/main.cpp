#include <iostream>

#include "Options.hpp"
#include "Map.hpp"
#include "Drawer.hpp"
#include "Evolution.hpp"

int main(int argc, char* argv[]) {
	Options::configure(argc, argv);

	Map::loadMap();
	Map::showMap();

	#ifndef NDEBUG
	Drawer::init(Map::points);
	#endif

	Evolution::start();

	// Drawer::draw();
	// Drawer::wait();

	return 0;
}