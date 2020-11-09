#include <iostream>
#include <iomanip>
#include <getopt.h>

#include "Options.hpp"
#include "Map.hpp"
#ifdef DEBUG
#include "Drawer.hpp"
#endif

int timeLimit = 100;

void parseArgs(int argc, char* argv[]) {
	const char usagestr[] = 
		"Usage: random-simulations [OPTIONS]... [TIME LIMIT] [MAP NUMBER]";

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
	if (rest > 2) {
		std::cerr << usagestr;
		exit(EXIT_FAILURE);
	}
	if (rest > 0)
		timeLimit = std::stoi(argv[optind++]);
	if (rest > 1)
		Options::mapNumber = std::stoi(argv[optind++]);
}

class Simulator {
public:
	static void simulate() {
		Timer timer(timeLimit);
		Agent agent;
		int simulationsDone = 0;
		int stepsTotal = 0;

		while (timer.isTimeLeft()) {
			Map::init(agent);
			while (Map::isFlying(agent)) {
				Action action = Action::getRandom();
				agent.apply(action);
				++stepsTotal;
			}
			++simulationsDone;
		}

		std::cout << std::fixed << std::setprecision(2);
		std::cout << "\nDone " << simulationsDone << " simulations in " << timeLimit << " ms" << "\n";
		std::cout << "Average simulation length: " << stepsTotal / double(simulationsDone) << "\n\n";
	}
};

int main(int argc, char* argv[]) {
	parseArgs(argc, argv);
	Map::load();

	#ifdef DEBUG
	Drawer::init();
	#endif

	Simulator::simulate();

	#ifdef DEBUG
	Drawer::draw();
	#endif

	return 0;
}
