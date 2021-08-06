#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

struct Player {
	sf::Vector2f position, speed;

	Player(sf::Vector2f _position) : position(_position) {
		speed = { 0, 0 };
	}

	Player(float x, float y) {
		position = { x, y };
		speed = { 0, 0 };
	}

	void check_movement() {
		speed.x += sf::Keyboard::isKeyPressed(sf::Keyboard::D) * 5.0f;
		speed.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A) * 5.0f;

		speed.y += sf::Keyboard::isKeyPressed(sf::Keyboard::S) * 5.0f;
		speed.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::W) * 5.0f;
			
	}

	void move() {
		position += speed;
	}

	void draw(sf::RenderWindow &window) {
		sf::RectangleShape shape(position);
		shape.setSize({ 40, 40 });
		shape.setPosition(position);
		shape.setFillColor(sf::Color::Green);
		window.draw(shape);
	}

};
