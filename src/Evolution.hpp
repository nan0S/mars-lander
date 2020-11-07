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
	static void printPopulation();

	static void evolution();
	static void selectParents();
	static float evaluatePopulation();
	static int selectParent(float x);
	static void crossover();
	static void cross(const Chromosome& p1, const Chromosome& p2, 
		Chromosome& c1, Chromosome& c2);
	static void mutation();
	static void mutate(Chromosome& ch);
	static Action chooseAction();
	static void makeStep();

	static void printGenerationStats(int generation);

	#ifdef VISUAL
	static void recordGeneration();
	#endif

	static Population population1;
	static Population population2;

	static Population* population;
	static Population* children;

	static Agent agent;
	static float evolutionTimeLimit;
	static float fitness[POPL];
	static int parentIdx[POPL];

	static int elite;
	static float mutationProb;
	static int random;

	static int mutations;
};

typedef RHEA Evolution;

#endif /* EVOLUTION_HPP */