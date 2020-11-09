#include "Drawer.hpp"

#include <cassert>
#include <unistd.h>

std::vector<sf::Vertex> Drawer::land;

sf::RenderWindow Drawer::window;

std::vector<std::vector<std::vector<sf::Vertex>>> Drawer::iterations;
std::vector<std::vector<sf::Vertex>> Drawer::generations;
std::vector<sf::Vertex> Drawer::generation;
std::vector<sf::Vertex> Drawer::sealed;

void Drawer::init() {
	land.reserve(Map::points.size());
	for (const auto& mapPoint : Map::points)
		land.emplace_back(transform(mapPoint), sf::Color::Red);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(W, H), "Mars Lander", sf::Style::Titlebar, settings);

	auto desktopMode = sf::VideoMode::getDesktopMode();
	window.setPosition(sf::Vector2i(desktopMode.width / 2, desktopMode.height / 2));

	sf::View view(sf::FloatRect(0, 0, Map::RIGHT_BORDER, Map::TOP_BORDER));
	window.setView(view);

	window.clear(sf::Color::Black);
	window.draw(land.data(), land.size(), sf::LinesStrip);
	window.display();
}

template<typename T>
sf::Vector2f Drawer::transform(const std::pair<T, T>& p) {
	return sf::Vector2f(float(p.x), float(Map::TOP_BORDER - p.y));
}

void Drawer::record(const Agent& agent) {
	generation.emplace_back(transform(agent.lastPos), sf::Color::White);
	generation.emplace_back(transform(agent.pos), sf::Color::White);
}

void Drawer::endGeneration() {
	generations.push_back(generation);
	generation.clear();
}

void Drawer::seal(const Agent& agent) {
	sealed.emplace_back(transform(agent.lastPos), sf::Color::Green);
	sealed.emplace_back(transform(agent.pos), sf::Color::Green);

	iterations.push_back(generations);
	generations.clear();
}

void Drawer::draw() {
	assert(iterations.size() * 2 == sealed.size());

	int iterationIdx = 0;
	for (const auto& iteration : iterations) {
		// std::cout << "Iteration: " << iterationIdx << std::endl;
		iterationIdx++;

		int generationIdx = 0;
		if (iterationIdx == 1) {
			while (generationIdx < std::min(20, int(iteration.size()))) {
				const auto& generation = iteration[generationIdx];
				// std::cout << "\tGeneration: " << generationIdx << std::endl;

				window.clear(sf::Color::Black);
				window.draw(land.data(), land.size(), sf::LinesStrip);
				window.draw(generation.data(), generation.size(), sf::Lines);
				window.draw(sealed.data(), 2 * iterationIdx, sf::Lines);
				window.display();

				usleep(50 * 1000);
				++generationIdx;
			}
		}

		for (; generationIdx < int(iteration.size()); generationIdx += 50) {
			const auto& generation = iteration[generationIdx];
			// std::cout << "\tGeneration: " << generationIdx << std::endl;

			window.clear(sf::Color::Black);
			window.draw(land.data(), land.size(), sf::LinesStrip);
			window.draw(generation.data(), generation.size(), sf::Lines);
			window.draw(sealed.data(), 2 * iterationIdx, sf::Lines);
			window.display();

			usleep(50 * 1000);
		}
	}

	window.clear(sf::Color::Black);
	window.draw(land.data(), land.size(), sf::LinesStrip);
	window.draw(sealed.data(), sealed.size(), sf::Lines);
	window.display();

	while (window.isOpen()) {
		sf::Event e;
		if (window.waitEvent(e)) {
			switch (e.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					switch (e.key.code) {
						case sf::Keyboard::Escape:
							window.close();
						default:
							break;
					}
				default:
					break;
			}
		}
	}
}
