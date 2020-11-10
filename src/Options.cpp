#include "Options.hpp"

#include <iostream>
#include <getopt.h>
#include <iomanip>

int Options::mapNumber = 1;
float Options::evolutionTimeLimit = 100;
bool Options::verbose = false;
bool Options::superVerbose = false;

float Options::mutationProb = 0.05f;
float Options::eliteFactor = 0.2f;

#ifdef LOCAL
void Options::configure(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(5);

	const char usagestr[] = 
		"Usage: mars-lander [OPTIONS]...";

	const char helpstr[] =
		"Run Mars Lander program.\n\n"
		"List of possible options:\n"
		"\t-m, --map\tspecify map number (default=1)\n"
		"\t-t, --tle\tspecify evolution time limit in milliseconds (default=40)\n"
		"\t-x, --mutation\tspecify mutation probability (default=5%)\n"
		"\t-e, --elite\tspecify elite factor (default=20%)\n"
		"\t-v, --verbose\tprint verbosely (default=false)\n"
		"\t-s, --super\tprint super verbosely (default=false)\n"
		"\t-h, --help\tprint this help\n\n";

	option longopts[] {
		{ "map", required_argument, 0, 'm' },
		{ "tle", required_argument, 0, 't' },
		{ "mutation", required_argument, 0, 'x' },
		{ "elite", required_argument, 0, 'e' },
		{ "verbose", no_argument, 0, 'v' },
		{ "super", no_argument, 0, 's' },
		{ "help", no_argument, 0, 'h' },
	};

	int idx, opt;
	while ((opt = getopt_long(argc, argv, "m:t:x:e:vsh", longopts, &idx)) != -1)
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
			case 'e':
				eliteFactor = std::stof(optarg);
				break;
			case 'v':
				verbose = true;
				break;
			case 's':
				superVerbose = true;
				break;
			case 'h':
				std::cout << '\n' << usagestr << "\n\n" << helpstr;
				exit(EXIT_SUCCESS);
		}

	#ifdef DEBUG
	if (verbose)
		show();
	#endif
}
#endif

#ifdef DEBUG
void Options::show() {
	std::cout << "\nOptions configuration:\n";
	std::cout << "\tmapNumber = " << mapNumber << "\n";
	std::cout << "\tevolutionTimeLimit = " << evolutionTimeLimit << "\n";
	std::cout << "\tmutationProb = " << mutationProb << "\n";
}
#endif
