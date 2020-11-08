#ifndef OPTIONS_HPP
#define OPTIONS_HPP

namespace Options {
	extern int mapNumber;
	extern float evolutionTimeLimit;
	extern bool verbose;
	extern float mutationProb;

	void configure(int argc, char* argv[]);
	#ifndef NDEBUG
	void show();
	#endif
}
	
#endif /* OPTIONS_HPP */