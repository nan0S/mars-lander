#include "Evolution.hpp"

#include "Map.hpp"
#include "Common.hpp"
#include "Options.hpp"

#ifndef NDEBUG
#include"Drawer.hpp"
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
}

void Evolution::start() {
	init();

	#ifndef NDEBUG
	if (Options::verbose)
		printPopulation();
	#endif

	while (!Map::isCrashed(agent)) {
		evolution();
		auto action = chooseAction();
		agent.apply(action);

		#ifndef NDEBUG
		if (Options::verbose) {
			Drawer::record(agent, sf::Color::Green);
			getchar();
		}
		#endif
	}
}

void Evolution::evolution() {
	Timer timer(evolutionTimeLimit);
	int generations = 0;
	int cnt = 30;

	#ifdef NDEBUG
	while (timer.isTimeLeft()) {
	#else
	while (cnt--) {
	#endif
		selectParents();
		crossover();
		mutation();
		std::swap(population, children);
		++generations;

		#ifndef NDEBUG
		std::cout << "Generation: " << generations << std::endl;
		Drawer::clear();
		#endif
	}

	#ifndef NDEBUG
	std::cout << "Generations: " << generations << std::endl;
	#endif
}

void Evolution::selectParents() {
	float sum = evaluatePopulation();

	static int idx[POPL];
	std::iota(idx, idx + POPL, 0);
	std::sort(idx, idx + POPL, [](const int& i, const int& j){
		return fitness[j] < fitness[i];
	});

	#ifndef NDEBUG
	if (Options::verbose) {
		std::cout << "idx: ";
		for (int i = 0; i < POPL; ++i)
			std::cout << idx[i] << " ";
		std::cout << std::endl;
	}
	#endif

	// elitism
	for (int i = 0; i < elite; ++i)
		parentIdx[i] = idx[i];

	// roulette selection
	for (int i = 1; i < POPL; ++i)
		fitness[i] += fitness[i - 1];
	for (int i = elite; i < POPL; ++i)
		parentIdx[i] = selectParent(Random::rand(sum));
}

float Evolution::evaluatePopulation() {
	float sum = 0;
	for (int i = 0; i < POPL; ++i) {
		Agent agent = Evolution::agent;
		const auto& chromosome = population->chromosomes[i];

		for (const auto& action : chromosome.genes) {
			agent.apply(action);
			#ifndef NDEBUG
			Drawer::record(agent);
			#endif
			if (Map::isCrashed(agent))
				break;
		}

		sum += (fitness[i] = Map::evaluate(agent));
	}

	#ifndef NDEBUG
	if (Options::verbose) {
		std::cout << "Fitness values:\n";
		for (int i = 0; i < POPL; ++i)
			std::cout << "\t" << i << ": " << fitness[i] << "\n";
	}
	#endif

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
	float p = Random::rand(1.0f);
	float q = 1 - p;

	for (int i = 0; i < CHL; ++i) {
		blend(p1.genes[i], p2.genes[i], c1.genes[i], p, q);
		blend(p1.genes[i], p2.genes[i], c2.genes[i], q, p);
	}
}

void Evolution::mutation() {
	for (auto& chromosome : population->chromosomes)
		if (Random::rand(1.0f) <= mutationProb)
			mutate(chromosome);
}

void Evolution::mutate(Chromosome& ch) {
	int geneIdx = Random::rand(CHL);
	ch.genes[geneIdx] = Action::getRandom();
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