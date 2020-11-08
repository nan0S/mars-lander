#ifndef EVOLUTION_HPP
#define EVOLUTION_HPP

#include "Agent.hpp"

#ifndef CHL
#define CHL 10
#endif
#ifndef POPL
#define POPL 50
#endif

static_assert(POPL % 2 == 0, "Population size must be divisible by 2!");

typedef Action Gene;
typedef struct {
	Gene genes[CHL];
} Chromosome;
typedef struct {
	Chromosome chromosomes[POPL];
} Population;

class RHEA {
public:
	static void start();

private:
	static void init();
	static void initPopulation();
	#ifndef NDEBUG
	static void printConfig();
	#endif
	static void evolution();
	static void selectParents();
	static void evaluatePopulation();
	static int selectParent(float x);
	static void crossover();
	static void cross(const Chromosome& p1, const Chromosome& p2,
		Chromosome& c1, Chromosome& c2);
	static void mutation();
	static void mutate(Chromosome& c);
	#ifndef NDEBUG
	static void printGenerationStats(int generation);
	static void recordGeneration();
	#endif
	static Action chooseAction();
	static void makeStep();

private:
	static Population population1;
	static Population population2;
	static Population* population;
	static Population* children;

	static Agent currentAgent;

	static float evolutionTimeLimit;
	static int elite;
	static int random;
	static float mutationProb;

	static int mutationCount;

	static float fitness[POPL];
	static int parentIdx[POPL];
};

typedef RHEA Evolution;

#endif /* EVOLUTION_HPP */