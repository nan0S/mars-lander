#include "Options.hpp"

#include "Common.hpp"

#include <getopt.h>
#include <iostream>
#include <unistd.h>

int Options::mapNumber = 1;
float Options::evolutionTimeLimit = 40;

float Options::eliteFactor = 0.2f;
float Options::mutationProb = 0.01f;
bool Options::verbose = false;

void Options::configure(int argc, char* argv[]) {
	std::cout.imbue(std::locale(""));

	const char usagestr[] = 
		"Usage: mars-lander [OPTIONS]...";

	const char helpstr[] =
		"Run Mars Lander program.\n\n"
		"List of possible options:\n"
		"\t-m, --map\tspecify map number\n"
		"\t-t, --tle\tspecify evolution time limit\n"
		"\t-x, --mutation\tspecify mutation probability\n"
		"\t-e, --elite\tspecify elitism factor\n"
		"\t-v, --verbose\tprint verbosely\n"
		"\t-h, --help\tprint this help\n\n";

	option longopts[] {
		{ "map", required_argument, 0, 'm' },
		{ "tle", required_argument, 0, 't' },
		{ "mutation", required_argument, 0, 'x' },
		{ "elite", required_argument, 0, 'e' },
		{ "verbose", no_argument, 0, 'v' },
		{ "help", no_argument, 0, 'h' },
	};

	int idx, opt;
	while ((opt = getopt_long(argc, argv, "m:t:x:e:vh", longopts, &idx)) != -1)
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
			case 'h':
				std::cout << '\n' << usagestr << "\n\n" << helpstr;
				exit(EXIT_SUCCESS);
		}

	if (verbose)
		show();
}

void Options::show() {
	std::cout << std::endl;
	std::cout << "mapNumber = " << mapNumber << "\n";
	std::cout << "evolutionTimeLimit = " << evolutionTimeLimit << "\n";
	std::cout << "eliteFactor = " << eliteFactor << "\n";
	std::cout << "mutationProb = " << mutationProb;

	getchar();
}