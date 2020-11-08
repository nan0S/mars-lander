#include "Evolution.hpp"
#include "Map.hpp"
#include "Options.hpp"
#ifndef NDEBUG
#include "Drawer.hpp"
#endif

#include <cassert>
#include <algorithm>

Population Evolution::population1;
Population Evolution::population2;
Population* Evolution::population;
Population* Evolution::children;

Agent Evolution::currentAgent;

float Evolution::evolutionTimeLimit;
int Evolution::elite;
int Evolution::random;
float Evolution::mutationProb;

int Evolution::mutationCount;

float Evolution::fitness[POPL];
int Evolution::parentIdx[POPL];

void Evolution::start() {
	init();
	
	for (int algorithmIteration = 0; !Map::isCrashed(currentAgent); ++algorithmIteration) {
		evolution();
		auto action = chooseAction();
		currentAgent.apply(action);

		#ifndef NDEBUG
		if (Options::verbose) {
			Drawer::seal(currentAgent);
			std::cout << "Iteration: " << algorithmIteration << std::endl;
		}
		#endif
	}	

	#ifndef NDEBUG
	if (Options::verbose)
		Drawer::draw();
	#endif
}

void Evolution::init() {
	initPopulation();
	Map::init(currentAgent);

	evolutionTimeLimit = Options::evolutionTimeLimit;
	elite = Options::eliteFactor * POPL;
	random = Options::randomFactor * POPL;
	mutationProb = Options::mutationProb;

	#ifndef NDEBUG
	if (Options::verbose)
		printConfig();
	#endif

	assert(random + elite <= POPL);
}

void Evolution::initPopulation() {
	population = &population1;
	children = &population2;

	for (auto& chromosome : population->chromosomes)
		for (auto& gene : chromosome.genes)
			gene = Gene::getRandom();
}

#ifndef NDEBUG
void Evolution::printConfig() {
	std::cout << "\nEvolution algorithm configuration:\n";
	std::cout << "\tAlgorithm: RHEA\n";
	std::cout << "\tEvolution time limit: " << evolutionTimeLimit << "\n";
	std::cout << "\tElite count: " << elite << "\n";
	std::cout << "\tRandom count: " << random << "\n";
	std::cout << "\tMutation probability: " << mutationProb << std::endl;
}
#endif

void Evolution::evolution() {
	Timer timer(evolutionTimeLimit);

	for (int generation = 0; timer.isTimeLeft(); ++generation) {
		mutationCount = 0;
		selectParents();
		crossover();
		mutation();
		std::swap(population, children);

		#ifndef NDEBUG
		if (Options::verbose) {
			recordGeneration();
			printGenerationStats(generation);
		}
		#endif
	}
}

void Evolution::selectParents() {
	evaluatePopulation();

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
	for (int i = elite; i < elite + random; ++i) {
		int randIdx = Random::rand(POPL);
		assert(0 <= randIdx && randIdx < POPL);
		parentIdx[i] = randIdx;
	}

	// roulette selection
	for (int i = 0; i < POPL - 1; ++i)
		fitness[i + 1] += fitness[i];

	assert(std::fabs(fitness[POPL - 1] - 1) <= 1e-4);
	fitness[POPL - 1] = 1;

	for (int i = elite + random; i < POPL; ++i) {
		float val = Random::rand<float>();
		assert(0 <= val && val <= 1);
		parentIdx[i] = selectParent(val);
	}

	std::shuffle(parentIdx, parentIdx + POPL, Random::rng);
	for (int i = 0; i < POPL; ++i)
		assert(parentIdx[i] < POPL);
}

void Evolution::evaluatePopulation() {
	float sum = 0.f;
	for (int i = 0; i < POPL; ++i) {
		Agent agent = currentAgent;
		const auto& chromosome = population->chromosomes[i];

		for (const auto& action : chromosome.genes) {
			agent.apply(action);
			if (Map::isCrashed(agent))
				break;
		}

		sum += (fitness[i] = Map::evaluate(agent));
	}

	for (int i = 0; i < POPL; ++i)
		fitness[i] /= sum;

	// float sum2 = 0.f;
	// for (int i = 0; i < POPL; ++i) {
	// 	fitness[i] = 1.f - fitness[i] / sum;
	// 	sum2 += fitness[i];
	// }
	// for (int i = 0; i < POPL; ++i)
	// 	fitness[i] /= sum2;
}

int Evolution::selectParent(float x) {
	for (int i = 0; i < POPL; ++i)
		if (x <= fitness[i])
			return i;
	assert(false);
}

void Evolution::crossover() {
	for (int i = 0; i < POPL; i += 2) {
		int& i1 = parentIdx[i];
		int& i2 = parentIdx[i + 1];
		cross(population->chromosomes[i1], population->chromosomes[i2],
			children->chromosomes[i], children->chromosomes[i + 1]);
	}
}

// inline void blend(const Gene& p1, const Gene& p2, Gene& c, float& p, float& q) {
// 	c.dAngle = clamp<int>(p1.dAngle * p + p2.dAngle * q, Action::MIN_DANGLE, Action::MAX_DANGLE);
// 	c.dThrust = clamp<int>(p1.dThrust * p + p2.dThrust * q, Action::MIN_DTHRUST, Action::MAX_DTHRUST);
// }

void Evolution::cross(const Chromosome& p1, const Chromosome& p2,
	Chromosome& c1, Chromosome& c2) {
	// float p = Random::rand(1.0f);
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
	for (auto& chromosome : children->chromosomes)
		if (Random::rand<float>() <= mutationProb)
			mutate(chromosome);
}

void Evolution::mutate(Chromosome& c) {
	++mutationCount;
	for (int i = 0; i < CHL; ++i)
		c.genes[i] = Gene::getRandom();
}

#ifndef NDEBUG
void Evolution::printGenerationStats(int generation) {
	evaluatePopulation();
	std::cout << "\tGeneration: " << generation + 1 << "\n";
	std::cout << "\t\tMin: " << *std::min_element(fitness, fitness + POPL) << " ";
	std::cout << "Avg: " << std::accumulate(fitness, fitness + POPL, 0.f) / POPL << " ";
	std::cout << "Max: " << *std::max_element(fitness, fitness + POPL) << "\n";
	std::cout << "\t\tMutation rate: " << float(mutationCount) / POPL << std::endl;
}

void Evolution::recordGeneration() {
	for (int i = 0; i < POPL; ++i) {
		Agent agent = currentAgent;
		const auto& chromosome = population->chromosomes[i];

		for (const auto& action : chromosome.genes) {
			agent.apply(action);
			Drawer::record(agent);
			if (Map::isCrashed(agent))
				break;
		}
	}
	Drawer::endGeneration();
}
#endif

Action Evolution::chooseAction() {
	evaluatePopulation();

	int bestIndividual = std::max_element(fitness, fitness + POPL) - fitness;
	const auto& bestAction = population->chromosomes[bestIndividual].genes[0];

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