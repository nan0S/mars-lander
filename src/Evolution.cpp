#include "Evolution.hpp"

#include "Map.hpp"
#include "Common.hpp"
#include "Options.hpp"

#ifdef VISUAL
#include "Drawer.hpp"
#endif

#include <cassert>
#include <algorithm>

Population Evolution::population1;
Population Evolution::population2;

Population* Evolution::population;
Population* Evolution::children;

Agent Evolution::agent;
float Evolution::evolutionTimeLimit;
float Evolution::fitness[POPL];
int Evolution::parentIdx[POPL];

int Evolution::elite;
float Evolution::mutationProb;
int Evolution::random;

int Evolution::mutations;

void Evolution::init() {
	population = &population1;
	children = &population2;

	for (int i = 0; i < POPL; ++i)
		for (int j = 0; j < CHL; ++j)
			population->chromosomes[i].genes[j] = Action::getRandom();

	Map::init(agent);

	Evolution::evolutionTimeLimit = Options::evolutionTimeLimit;
	Evolution::elite = Options::eliteFactor * POPL;
	Evolution::mutationProb = Options::mutationProb;
	Evolution::random = Options::randomnessFactor * POPL;

	assert(random + elite <= POPL);
}

void Evolution::start() {
	init();
	int iteration = 0;

	while (!Map::isCrashed(agent)) {
		#ifdef VISUAL
		Drawer::seal(agent);
		#endif

		evolution();
		auto action = chooseAction();
		agent.apply(action);

		#ifdef VISUAL
		Drawer::endIteration();
		#endif

		++iteration;
		#ifndef NDEBUG
		if (Options::verbose)
			std::cout << "Iteration: " << iteration << std::endl;
		#endif
	}

	#ifdef VISUAL
	Drawer::seal(agent);
	#endif
}

void Evolution::evolution() {
	Timer timer(evolutionTimeLimit);
	int generation = 0;

	while (timer.isTimeLeft()) {
		#ifdef VISUAL
		recordGeneration();
		#endif

		mutations = 0;

		selectParents();
		crossover();
		mutation();
		std::swap(population, children);
		++generation;

		#ifndef NDEBUG
		if (Options::verbose)
			printGenerationStats(generation);
		#endif
	}

	#ifdef VISUAL
	recordGeneration();
	#endif

	#ifndef NDEBUG
	if (Options::verbose)
		std::cout << "\tGenerations: " << generation << std::endl;
	#endif
}

void Evolution::selectParents() {
	float sum = evaluatePopulation();

	static int idx[POPL];
	std::iota(idx, idx + POPL, 0);
	std::sort(idx, idx + POPL, [](const int& i, const int& j){
		return fitness[j] < fitness[i];
	});

	for (int i = 0; i < POPL - 1; ++i)
		assert(fitness[idx[i]] >= fitness[idx[i + 1]]);

	// elitism
	for (int i = 0; i < elite; ++i)
		parentIdx[i] = idx[i];

	// random
	for (int i = elite; i < elite + random; ++i)
		parentIdx[i] = Random::rand(POPL);

	// roulette selection
	for (int i = 1; i < POPL; ++i)
		fitness[i] += fitness[i - 1];
	for (int i = elite + random; i < POPL; ++i)
		parentIdx[i] = selectParent(Random::rand(sum));

	std::shuffle(parentIdx, parentIdx + POPL, Random::rng);
	for (int i = 0; i < POPL; ++i)
		assert(parentIdx[i] < POPL);
}

float Evolution::evaluatePopulation() {
	float sum = 0;
	for (int i = 0; i < POPL; ++i) {
		Agent agent = Evolution::agent;
		const auto& chromosome = population->chromosomes[i];

		for (const auto& action : chromosome.genes) {
			agent.apply(action);
			if (Map::isCrashed(agent))
				break;
		}

		sum += (fitness[i] = Map::evaluate(agent));
	}

	return sum;
}

int Evolution::selectParent(float x) {
	for (int i = 0; i < POPL; ++i)
		if (x <= fitness[i])
			return i;
	assert(false);
}

void Evolution::crossover() {
	for (int i = 0; i < POPL; i += 2) {
		auto& i1 = parentIdx[i], &i2 = parentIdx[i + 1];
		cross(population->chromosomes[i1], population->chromosomes[i2],
			children->chromosomes[i], children->chromosomes[i + 1]);
	}
}

inline void blend(const Gene& p1, const Gene& p2, Gene& c, float& p, float& q) {
	c.dAngle = clamp<int>(p1.dAngle * p + p2.dAngle * q, Action::MIN_DANGLE, Action::MAX_DANGLE);
	c.dThrust = clamp<int>(p1.dThrust * p + p2.dThrust * q, Action::MIN_DTHRUST, Action::MAX_DTHRUST);
}

void Evolution::cross(const Chromosome& p1, const Chromosome& p2, Chromosome& c1, Chromosome& c2) {
	// float p = Random::rand(1.0f);
	// assert(p >= 0.f && p <= 1.f);
	// float q = 1 - p;

	// for (int i = 0; i < CHL; ++i) {
	// 	blend(p1.genes[i], p2.genes[i], c1.genes[i], p, q);
	// 	blend(p1.genes[i], p2.genes[i], c2.genes[i], q, p);
	// }

	int crossIdx = Random::rand(CHL + 1);
	assert(0 <= crossIdx && crossIdx <= CHL);

	for (int i = 0; i < crossIdx; ++i) {
		c1.genes[i] = p1.genes[i];
		c2.genes[i] = p2.genes[i];
	}
	for (int i = crossIdx; i < CHL; ++i) {
		c1.genes[i] = p2.genes[i];
		c2.genes[i] = p1.genes[i];
	}
}

void Evolution::mutation() {
	for (auto& chromosome : population->chromosomes)
		if (Random::rand(1.0f) <= mutationProb)
			mutate(chromosome);
}

void Evolution::mutate(Chromosome& ch) {
	++mutations;

	int geneIdx = Random::rand(CHL);
	assert(geneIdx < CHL);
	for (int i = 0; i < CHL; ++i)
		ch.genes[i] = Action::getRandom();
	// ch.genes[geneIdx] = Action::getRandom(j);
}

void Evolution::printPopulation() {
	int i = 1;
	for (const auto& chromosome : population->chromosomes) {
		std::cout << "Individual: " << i++ << std::endl;
		for (const auto& gene : chromosome.genes) {
			std::cout << "\t" << gene << "\n";
		}
	}
}

Action Evolution::chooseAction() {
	evaluatePopulation();

	int bestIndividual = std::max_element(fitness, fitness + POPL) - fitness;
	Action bestAction = population->chromosomes[bestIndividual].genes[0];

	makeStep();

	return bestAction;
}

void Evolution::makeStep() {
	for (auto& chromosome : population->chromosomes) {
		for (int i = 0; i < CHL - 1; ++i)
			chromosome.genes[i] = chromosome.genes[i + 1];
		chromosome.genes[CHL - 1] = Gene::getRandom();
	}
}

void Evolution::printGenerationStats(int generation) {
	evaluatePopulation();
	std::cout << "\tGeneration: " << generation << std::endl;
	std::cout << "\t\tMin: " << *std::min_element(fitness, fitness + POPL) << " ";
	std::cout << "Avg: " << std::accumulate(fitness, fitness + POPL, 0.f) / POPL << " ";
	std::cout << "Max: " << *std::max_element(fitness, fitness + POPL) << std::endl;
	std::cout << "\t\tMutation rate: " << float(mutations) / POPL << std::endl;
}

#ifdef VISUAL
void Evolution::recordGeneration() {
	for (int i = 0; i < POPL; ++i) {
		Agent agent = Evolution::agent;
		const auto& chromosome = population->chromosomes[i];

		for (const auto& action : chromosome.genes) {
			Drawer::record(agent);
			agent.apply(action);
			if (Map::isCrashed(agent))
				break;
		}
		Drawer::record(agent);
	}
	Drawer::endGeneration();
}
#endif