#include "Agent.hpp"
#include "Map.hpp"

#ifndef NDEBUG
#include "Drawer.hpp"
#endif

#include <getopt.h>

int timeLimit = 100;
int simLength = 50;
int mapNr = 1;

void parseArgs(int argc, char* argv[]) {
	const char usagestr[] = 
		"Usage: random-simulations [OPTIONS]... [TIME LIMIT] [SIMULATION LENGTH] [MAP NUMBER]";

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
	if (rest > 3)
		errorExit(usagestr);
	if (rest > 0)
		timeLimit = std::stoi(argv[optind++]);
	if (rest > 1)
		simLength = std::stoi(argv[optind++]);
	if (rest > 2)
		mapNr = std::stoi(argv[optind++]);
}

class Simulator {
public:
	static void simulate(const int timeLimit, const int simLength) {
		Timer timer(timeLimit);
		int simulationsDone = 0;
		Agent agent;

		#ifndef NDEBUG
		int cnt = 50;
		while (cnt--) {
		#else
		while (timer.isTimeLeft()) {
		#endif
			Map::init(agent);

			for (int j = 0; j < simLength; ++j) {
				Action action = Action::getRandom();
				agent.apply(action);

				#ifndef NDEBUG				
				Drawer::record(agent);
				#endif

				if (Map::isCrashed(agent))
					break;
			}

			++simulationsDone;
		}

		std::cout << "\nDone " << simulationsDone << " simulations in " << timeLimit << " ms" << std::endl;
		std::cout << "Simulation length = " << simLength << std::endl << std::endl; 
	}
};

int main(int argc, char* argv[]) {
	std::cout.imbue(std::locale(""));
	parseArgs(argc, argv);

	Map::loadMap();

	#ifndef NDEBUG
	Drawer::init(Map::points);
	Drawer::draw();
	#endif

	Simulator::simulate(timeLimit, simLength);

	#ifndef NDEBUG
	Drawer::wait();
	#endif

	return 0;
}