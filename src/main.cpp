#include "Common.hpp"
#include "Options.hpp"
#include "Map.hpp"
#include "Evolution.hpp"

#ifdef DEBUG
#include "Drawer.hpp"
#endif

int main(int argc, char* argv[]) {
	#ifdef LOCAL
	Options::configure(argc, argv);
	#endif

	Map::load();

	#ifdef DEBUG
	if (Options::verbose)
		Drawer::init();
	#endif

	Evolution::start();

	return 0;
}
