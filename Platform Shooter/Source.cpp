#include <SFML/Graphics.hpp>
#include <chrono>

#include "Player.h"

int main() {
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Platform Shooter", sf::Style::Fullscreen);

	Player player(400, 400);

	sf::Event event;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	while (window.isOpen()) {

		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
			}
		}

		player.check_movement();
		player.move();

		window.clear();

		player.draw(window);

		window.display();
	}

	return 0;
}
