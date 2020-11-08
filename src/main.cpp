#include "Options.hpp"
#include "Map.hpp"
#include "Evolution.hpp"
#include "Drawer.hpp"

int main(int argc, char* argv[]) {
	Options::configure(argc, argv);
	Map::load();

	#ifndef NDEBUG
	if (Options::verbose)
		Drawer::init();
	#endif

	Evolution::start();

	return 0;
}