#include "Evolution.hpp"
#include "Map.hpp"
#include "Options.hpp"

#ifdef DEBUG
#include "Drawer.hpp"
#endif

#include <cassert>
#include <algorithm>

Population Evolution::population1;
Population Evolution::population2;
Population* Evolution::population;
Population* Evolution::nextPopulation;

Agent Evolution::currentAgent;

float Evolution::evolutionTimeLimit;
float Evolution::mutationProb;
int Evolution::elite;

int Evolution::mutationCount;

float Evolution::fitness[POPL + OFFL];
float Evolution::objective[POPL + OFFL];
int Evolution::parentIdx[OFFL];

void Evolution::start() {
	init();
	
	#ifdef LOCAL
	int loops = 0;
	#endif

	for (int algorithmIteration = 0; Map::isFlying(currentAgent); ++algorithmIteration) {
		evolution();

		auto action = chooseAction();
		currentAgent.apply(action);

		#ifdef DEBUG
		if (Options::verbose) {
			Drawer::seal(currentAgent);
			std::cout << "Iteration: " << algorithmIteration << std::endl;
		}
		#endif

		#ifdef LOCAL
		++loops;
		#else
		std::cout << currentAgent.angle << " " << currentAgent.thrust << std::endl;
		std::cin >> currentAgent.pos.x >> currentAgent.pos.y;
		std::cin >> currentAgent.vel.x >> currentAgent.vel.y;
		std::cin >> currentAgent.fuel >> currentAgent.angle >> currentAgent.thrust;
		#endif
	}	

	#ifdef LOCAL
	std::cout << loops << "\n";
	if (Map::getShipState(currentAgent) == ShipState::Landed)
		std::cout << "Landed!\n";
	else
		std::cout << "Not landed!\n";
	#else
	for (int i = 0; i < 10; ++i)
		std::cout << "0 0" << std::endl;
	#endif

	#ifdef DEBUG
	if (Options::verbose) {
		std::cout << "\nFinal velocity: " << currentAgent.vel << "\n";
		std::cout << "Final angle: " << currentAgent.angle << "\n";
		std::cout << "Final fuel: " << currentAgent.fuel << "\n";
		std::cout << "Final position: " << currentAgent.pos << "\n";
		std::cout << "Final state: " << Map::getShipState(currentAgent) << std::endl;
		Drawer::draw();
	}
	#endif
}

void Evolution::init() {
	initPopulation();
	Map::init(currentAgent);

	evolutionTimeLimit = Options::evolutionTimeLimit;
	mutationProb = Options::mutationProb;
	elite = Options::eliteFactor * POPL;

	assert(0 <= elite && elite <= POPL);

	#ifdef DEBUG
	if (Options::verbose)
		printConfig();
	#endif
}

void Evolution::initPopulation() {
	population = &population1;
	nextPopulation = &population2;

	for (int i = 0; i < POPL; ++i) {
		auto& chromosome = population->chromosomes[i];
		for (auto& gene : chromosome.genes)
			gene = Gene::getRandom();
	}
}

#ifdef DEBUG
void Evolution::printConfig() {
	std::cout << "\nEvolution algorithm configuration:\n";
	std::cout << "\tAlgorithm: RHEA\n";
	std::cout << "\tEvolution time limit: " << evolutionTimeLimit << "\n";
	std::cout << "\tMutation probability: " << mutationProb << "\n";
	std::cout << "\tElite count: " << elite << std::endl;
}
#endif

void Evolution::evolution() {
	Timer timer(evolutionTimeLimit);
	
	// for (int generation = 0; generation < 50; ++generation) {
	for (int generation = 0; timer.isTimeLeft(); ++generation) {
		mutationCount = 0;
		selection();
		crossover();
		mutation();
		replacement();

		std::swap(population, nextPopulation);

		#ifdef DEBUG
		if (Options::verbose) {
			recordGeneration();
			printGenerationStats(generation);
		}
		#endif
	}
}

void Evolution::selection() {
	evaluatePopulation();

	for (int i = 0; i < POPL - 1; ++i)
		fitness[i + 1] += fitness[i];

	assert(std::fabs(fitness[POPL - 1] - 1) <= 1e-4);
	fitness[POPL - 1] = 1.f;

	for (int i = 0; i < OFFL; ++i) {
		float val = Random::rand<float>();
		assert(0.f <= val && val <= 1.f);
		parentIdx[i] = selectParent(val);
	}

	for (int i = 0; i < OFFL; ++i)
		assert(parentIdx[i] < POPL);
}

void Evolution::evaluatePopulation() {
	float maxObjective = 0.f;

	for (int i = 0; i < POPL; ++i) {
		Agent agent = currentAgent;
		const auto& chromosome = population->chromosomes[i];

		for (const auto& action : chromosome.genes) {
			agent.apply(action);
			if (!Map::isFlying(agent))
				break;
		}

		maxObjective = std::max(maxObjective, objective[i] = Map::evaluate(agent));
	}

	float sum = 0.f;
	for (int i = 0; i < POPL; ++i)
		sum += (objective[i] = maxObjective - objective[i]);

	if (sum == 0.f)
		for (int i = 0; i < POPL; ++i)
			fitness[i] = 1.f / POPL;
	else
		for (int i = 0; i < POPL; ++i)
			fitness[i] = objective[i] / sum;	
}

int Evolution::selectParent(float x) {
	for (int i = 0; i < POPL; ++i)
		if (x <= fitness[i])
			return i;
	assert(false);
}

void Evolution::crossover() {
	for (int i = 0; i < OFFL; i += 2) {
		int& i1 = parentIdx[i];
		int& i2 = parentIdx[i + 1];
		assert(0 <= i1 && i1 < POPL);
		assert(0 <= i2 && i2 < POPL);
		cross(population->chromosomes[i1], population->chromosomes[i2],
			population->chromosomes[POPL + i], population->chromosomes[POPL + i + 1]);
	}
}

static inline void blend(const Gene& p1, const Gene& p2, Gene& c, float& p, float& q) {
	c.dAngle = clamp<int>(p1.dAngle * p + p2.dAngle * q, Action::MIN_DANGLE, Action::MAX_DANGLE);
	c.dThrust = clamp<int>(p1.dThrust * p + p2.dThrust * q, Action::MIN_DTHRUST, Action::MAX_DTHRUST);
}

static void uniformCross(const Chromosome& p1, const Chromosome& p2,
	Chromosome& c1, Chromosome& c2) {
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

static void continuousCross(const Chromosome& p1, const Chromosome& p2,
	Chromosome& c1, Chromosome& c2) {
	float p = Random::rand(1.0f);
	float q = 1 - p;

	for (int i = 0; i < CHL; ++i) {
		blend(p1.genes[i], p2.genes[i], c1.genes[i], p, q);
		blend(p1.genes[i], p2.genes[i], c2.genes[i], q, p);
	}
}

static void shuffleCross(const Chromosome& p1, const Chromosome& p2,
	Chromosome& c1, Chromosome& c2) {
	for (int i = 0; i < CHL; ++i) {
		if (Random::rand<float>() <= 0.5f) {
			c1.genes[i] = p1.genes[i];
			c2.genes[i] = p2.genes[i];
		}
		else {
			c1.genes[i] = p2.genes[i];
			c2.genes[i] = p1.genes[i];
		}
	}
}

void Evolution::cross(const Chromosome& p1, const Chromosome& p2,
	Chromosome& c1, Chromosome& c2) {
	// continuousCross(p1, p2, c1, c2);
	shuffleCross(p1, p2, c1, c2);
	// uniformCross(p1, p2, c1, c2);
}

void Evolution::mutation() {
	for (int i = POPL; i < POPL + OFFL; ++i) {
		auto& chromosome = population->chromosomes[i];
		for (auto& gene : chromosome.genes)
			if (Random::rand<float>() <= mutationProb)
				mutate(gene);
	}
}

void Evolution::mutate(Gene& gene) {
	++mutationCount;
	gene = Gene::getRandom();
}

void Evolution::replacement() {
	static int idx[POPL];
	std::iota(idx, idx + POPL, 0);
	std::partial_sort(idx, idx + elite, idx + POPL, [](const int& i, const int& j){
		return objective[j] < objective[i];
	});

	for (int i = 0; i < elite - 1; ++i)
		assert(objective[idx[i]] >= objective[idx[i + 1]]);

	for (int i = 0; i < elite; ++i)
		nextPopulation->chromosomes[i] = population->chromosomes[idx[i]];
	for (int i = elite; i < POPL; ++i)
		nextPopulation->chromosomes[i] = population->chromosomes[POPL + i];
}

#ifdef DEBUG
void Evolution::printGenerationStats(int generation) {
	evaluatePopulation();
	std::cout << "\tGeneration: " << generation + 1 << "\n";
	std::cout << "\t\tMin: " << *std::min_element(objective, objective + POPL) << " ";
	std::cout << "Avg: " << std::accumulate(objective, objective + POPL, 0.f) / POPL << " ";
	std::cout << "Max: " << *std::max_element(objective, objective + POPL) << "\n";
	std::cout << "\t\tMutation rate: " << float(mutationCount) / (OFFL * CHL) << std::endl;
}

void Evolution::recordGeneration() {
	for (int i = 0; i < POPL; ++i) {
		Agent agent = currentAgent;
		const auto& chromosome = population->chromosomes[i];

		for (const auto& action : chromosome.genes) {
			agent.apply(action);
			Drawer::record(agent);
			if (!Map::isFlying(agent))
				break;
		}
	}
	Drawer::endGeneration();
}
#endif

Action Evolution::chooseAction() {
	evaluatePopulation();

	// std::cout << std::accumulate(objective, objective + POPL, 0.f) / POPL << ",";
	// std::cout << *std::max_element(objective, objective + POPL) << std::endl;

	int bestIndividual = std::max_element(fitness, fitness + POPL) - fitness;
	const auto bestAction = population->chromosomes[bestIndividual].genes[0];

	makeStep();

	return bestAction;
}

void Evolution::makeStep() {
	for (int i = 0; i < POPL; ++i) {
		auto& chromosome = population->chromosomes[i];
		for (int j = 0; j < CHL - 1; ++j)
			chromosome.genes[j] = chromosome.genes[j + 1];
		chromosome.genes[CHL - 1] = Gene::getRandom();
	}
}
