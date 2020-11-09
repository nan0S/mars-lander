#ifndef OPTIONS_HPP
#define OPTIONS_HPP

namespace Options {
	extern int mapNumber;
	extern float evolutionTimeLimit;
	extern bool verbose;
	extern bool superVerbose;
	extern float mutationProb;
	extern float eliteFactor;

	#ifdef LOCAL
	void configure(int argc, char* argv[]);
	#endif
	#ifdef DEBUG
	void show();
	#endif
}
	
#endif /* OPTIONS_HPP */
