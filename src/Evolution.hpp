#ifndef EVOLUTION_HPP
#define EVOLUTION_HPP

#include "Agent.hpp"

#ifndef POPL
#define POPL 50
#endif
#ifndef OFFL
#define OFFL POPL
#endif
#ifndef CHL
#define CHL 10
#endif

static_assert(OFFL % 2 == 0, "Offspring size must be divisible by 2!");

typedef Action Gene;
typedef struct {
	Gene genes[CHL];
} Chromosome;
typedef struct {
	Chromosome chromosomes[POPL + OFFL];
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
	static void selection();
	static void evaluatePopulation();
	static int selectParent(float x);
	static void crossover();
	static void cross(const Chromosome& p1, const Chromosome& p2,
		Chromosome& c1, Chromosome& c2);
	static void mutation();
	static void mutate(Gene& gene);
	static void replacement();
	static void evaluateChildren();
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
	static Population* nextPopulation;

	static Agent currentAgent;

	static float evolutionTimeLimit;
	static float mutationProb;
	static int elite;

	static int mutationCount;

	static float fitness[POPL + OFFL];
	static float objective[POPL + OFFL];
	static int parentIdx[OFFL];
};

typedef RHEA Evolution;

#endif /* EVOLUTION_HPP */