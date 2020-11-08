#include "Options.hpp"

#include <iostream>
#include <getopt.h>

int Options::mapNumber = 1;
float Options::evolutionTimeLimit = 40;
bool Options::verbose = false;

float Options::eliteFactor = 0.2f;
float Options::mutationProb = 0.05f;
float Options::randomFactor = 0.1f;

void Options::configure(int argc, char* argv[]) {
	const char usagestr[] = 
		"Usage: mars-lander [OPTIONS]...";

	const char helpstr[] =
		"Run Mars Lander program.\n\n"
		"List of possible options:\n"
		"\t-m, --map\tspecify map number\n"
		"\t-t, --tle\tspecify evolution time limit\n"
		"\t-x, --mutation\tspecify mutation probability\n"
		"\t-e, --elite\tspecify elitism factor\n"
		"\t-r, --randomness\t specify randomness factor\n"
		"\t-v, --verbose\tprint verbosely\n"
		"\t-h, --help\tprint this help\n\n";

	option longopts[] {
		{ "map", required_argument, 0, 'm' },
		{ "tle", required_argument, 0, 't' },
		{ "mutation", required_argument, 0, 'x' },
		{ "elite", required_argument, 0, 'e' },
		{ "randomness", required_argument, 0, 'r' },
		{ "verbose", no_argument, 0, 'v' },
		{ "help", no_argument, 0, 'h' },
	};

	int idx, opt;
	while ((opt = getopt_long(argc, argv, "m:t:x:e:r:vh", longopts, &idx)) != -1)
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
			case 'r':
				randomFactor = std::stof(optarg);
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
	std::cout << "\teliteFactor = " << eliteFactor << "\n";
	std::cout << "\tmutationProb = " << mutationProb << "\n";
	std::cout << "\trandomFactor = " << randomFactor << std::endl;
}
#endif