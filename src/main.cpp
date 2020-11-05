#include <iostream>
#include <getopt.h>

#include "Map.hpp"

int mapNr = 1;

void parseArgs(int argc, char* argv[]) {
	const char usagestr[] = 
		"Usage: mars-lander [OPTIONS]... [MAP NUMBER]";

	const char helpstr[] =
		"Run Mars Lander program.\n\n"
		"List of possible options:\n"
		"\t-h, --help\tprint this help\n\n";

	option longopts[] {
		{"help", no_argument, 0, 'h'}
	};

	int idx, opt;
	while ((opt = getopt_long(argc, argv, "h", longopts, &idx)) != -1)
		switch (opt) {
			case 'h':
				std::cout << '\n' << usagestr << "\n\n" << helpstr;
				exit(EXIT_SUCCESS);
		}

	int rest = argc - optind;
	if (rest > 1)
		errorExit(usagestr);
	if (rest > 0)
		mapNr = std::stoi(argv[optind++]);
}

int main(int argc, char* argv[]) {
	parseArgs(argc, argv);

	Map::loadMap(mapNr);
	Map::showMap();

	return 0;
}