#ifndef OPTIONS_HPP
#define OPTIONS_HPP

namespace Options {
	extern int mapNumber;
	extern float evolutionTimeLimit;

	extern bool verbose;
	extern float eliteFactor;
	extern float mutationProb;
	extern float randomnessFactor;

	void configure(int argc, char* argv[]);
	void show();
}

#endif /* OPTIONS_HPP */