#ifndef OPTIONS_HPP
#define OPTIONS_HPP

namespace Options {
	extern int mapNumber;
	extern float evolutionTimeLimit;
	extern bool verbose;
	extern bool superVerbose;
	extern float mutationProb;
	extern float eliteFactor;

	void configure(int argc, char* argv[]);
	#ifndef NDEBUG
	void show();
	#endif
}
	
#endif /* OPTIONS_HPP */