#include "Drawer.hpp"

std::vector<sf::Vertex> Drawer::map;
std::vector<sf::Vertex> Drawer::agents;
std::vector<sf::Vertex> Drawer::sealed;
int Drawer::step = 0;
sf::RenderWindow Drawer::window;

void Drawer::init(const std::vector<Point>& points) {
	map.reserve(points.size());
	for (const auto& point : points)
		map.emplace_back(transform(point), sf::Color::Red);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(W, H), "Mars Lander", sf::Style::Titlebar, settings);

	auto desktopMode = sf::VideoMode::getDesktopMode();
	window.setPosition(sf::Vector2i(desktopMode.width / 2, desktopMode.height / 2));

	sf::View view(sf::FloatRect(0, 0, Map::RIGHT_BORDER, Map::TOP_BORDER));
	window.setView(view);
}	

void Drawer::draw() {
	window.clear(sf::Color::Black);
	window.draw(map.data(), map.size(), sf::LinesStrip);
	window.draw(agents.data(), agents.size(), sf::Lines);
	window.draw(sealed.data(), agents.size(), sf::Lines);
	window.display();
}

void Drawer::wait() {
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

void Drawer::record(const Agent& agent, const sf::Color& color) {
	agents.emplace_back(transform(agent.lastPos), color);
	agents.emplace_back(transform(agent.pos), color);
	draw();
}

void Drawer::seal(const Agent& agent) {
	sealed.emplace_back(transform(agent.lastPos), sf::Color::Green);
	sealed.emplace_back(transform(agent.pos), sf::Color::Green);
	draw();
}

void Drawer::clear() {
	agents.clear();
	++step;
	draw();
} 