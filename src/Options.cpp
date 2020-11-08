#include "Options.hpp"

#include <iostream>
#include <getopt.h>
#include <iomanip>

int Options::mapNumber = 1;
float Options::evolutionTimeLimit = 40;
bool Options::verbose = false;

float Options::mutationProb = 0.05f;

void Options::configure(int argc, char* argv[]) {
	std::cout << std::fixed << std::setprecision(5);

	const char usagestr[] = 
		"Usage: mars-lander [OPTIONS]...";

	const char helpstr[] =
		"Run Mars Lander program.\n\n"
		"List of possible options:\n"
		"\t-m, --map\tspecify map number\n"
		"\t-t, --tle\tspecify evolution time limit\n"
		"\t-x, --mutation\tspecify mutation probability\n"
		"\t-v, --verbose\tprint verbosely\n"
		"\t-h, --help\tprint this help\n\n";

	option longopts[] {
		{ "map", required_argument, 0, 'm' },
		{ "tle", required_argument, 0, 't' },
		{ "mutation", required_argument, 0, 'x' },
		{ "verbose", no_argument, 0, 'v' },
		{ "help", no_argument, 0, 'h' },
	};

	int idx, opt;
	while ((opt = getopt_long(argc, argv, "m:t:x:vh", longopts, &idx)) != -1)
		switch (opt) {
			case 'm':
				mapNumber = std::stoi(optarg);
				break;
			case 't':
				evolutionTimeLimit = std::stof(optarg);
				break;
			case 'x':
				mutationProb = std::stof(optarg);
				break;
			case 'v':
				verbose = true;
				break;
			case 'h':
				std::cout << '\n' << usagestr << "\n\n" << helpstr;
				exit(EXIT_SUCCESS);
		}

	#ifndef NDEBUG
	if (verbose)
		show();
	#endif
}

#ifndef NDEBUG
void Options::show() {
	std::cout << "\nOptions configuration:\n";
	std::cout << "\tmapNumber = " << mapNumber << "\n";
	std::cout << "\tevolutionTimeLimit = " << evolutionTimeLimit << "\n";
	std::cout << "\tmutationProb = " << mutationProb << "\n";
}
#endif