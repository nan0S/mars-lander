#include <iostream>
#include <SFML/Graphics.hpp>

const int W = 1280;
const int H = 720;

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(W, H), "SFML", sf::Style::Titlebar, settings);

	while (window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Escape:
							window.close();
						default:
							break;
					}
					break;
				default:
					break;					
			}
		}

		window.clear(sf::Color::Black);
		window.display();
	}


	return 0;
}