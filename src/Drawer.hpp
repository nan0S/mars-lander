#ifndef DRAWER_HPP
#define DRAWER_HPP

#include "Common.hpp"
#include "Map.hpp"
#include "Agent.hpp"

#include <SFML/Graphics.hpp>

class Drawer {
public:
	static void init();
	static void record(const Agent& agent);
	static void endGeneration();
	static void seal(const Agent& agent);
	static void draw();

private:
	template<typename T>
	static sf::Vector2f transform(const std::pair<T, T>& p);

private:
	static std::vector<sf::Vertex> land;
	static sf::RenderWindow window;

	static std::vector<std::vector<std::vector<sf::Vertex>>> iterations;
	static std::vector<std::vector<sf::Vertex>> generations;
	static std::vector<sf::Vertex> generation;
	static std::vector<sf::Vertex> sealed;

	static constexpr int W = 1280;
	static constexpr int H = float(W) * Map::TOP_BORDER / Map::RIGHT_BORDER;
};

#endif /* DRAWER_HPP */