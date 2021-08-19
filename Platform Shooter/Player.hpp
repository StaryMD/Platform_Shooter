#pragma once

struct Player {
	sf::Vector2f pos, velocity;
	sf::Texture texture;
	sf::Sprite sprite;
	inline static sf::Vector2f size = { 0.9f, 1.8f };
	bool is_on_ground = true;

	Player(Map &map) {
		spawn_randomly(map);
		velocity = { 0.f, 0.f };
	}

	Player(const sf::Vector2f &_position) {
		pos = _position;
		velocity = { 0.f, 0.f };
	}

	Player(const float x, const float y) {
		pos = { x, y };
		velocity = { 0.f, 0.f };
	}

	void update(float &elapsed_time, Map &map, bool has_focus) {
		check_movement(elapsed_time, has_focus);
		move(map);
	}

	void move(Map &map) {
		sf::Vector2f new_pos = pos + velocity;

		if (velocity.x < 0.0f) { // going left
			if (map.is_solid(new_pos.x, pos.y) || map.is_solid(new_pos.x, pos.y + size.y * 0.5f) || map.is_solid(new_pos.x, pos.y + size.y)) {
				new_pos.x = std::truncf(new_pos.x + 1.0f);
				velocity.x = 0.0f;
			}
		}
		else { // going right
			if (map.is_solid(new_pos.x + size.x, pos.y) || map.is_solid(new_pos.x + size.x, pos.y + size.y * 0.5f) || map.is_solid(new_pos.x + size.x, pos.y + size.y)) {
				new_pos.x = std::truncf(new_pos.x) + 0.09999f;
				velocity.x = 0.0f;
			}
		}

		is_on_ground = false;
		if (velocity.y < 0.0f) { // going up
			if (map.is_solid(new_pos.x, new_pos.y) || map.is_solid(new_pos.x + size.x, new_pos.y)) {
				new_pos.y = std::truncf(new_pos.y + 1.0f);
				velocity.y = 0.0f;
			}
		}
		else { // going down
			if (map.is_solid(new_pos.x, new_pos.y + size.y) || map.is_solid(new_pos.x + size.x, new_pos.y + size.y)) {
				new_pos.y = std::truncf(new_pos.y) + 0.19999f;
				velocity.y = 0.0f;
				is_on_ground = true;
			}
		}

		pos = new_pos;
	}

	void check_movement(float &elapsed_time, bool has_focus) {
		static const float ground_step = 1.0f;
		static const float air_step = 0.2f;
		static const float jump_velocity = 10.0f;

		static const float gravity = 0.5f;
		static const float ground_friction = 7.f;
		static const float air_friction = 2.0f;

		static const float terminal_velocity = 14.0f;
		static const float terminal_velocity_on_ground = 6.0f;

		/*
		if (is_on_ground) {
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
				velocity.x += ground_step * elapsed_time;
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
				velocity.x -= ground_step * elapsed_time;

			velocity.x *= ground_friction;

			velocity.x = std::min(velocity.x,  terminal_velocity_on_ground);
			velocity.x = std::max(velocity.x, -terminal_velocity_on_ground);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				velocity.y = jump_velocity;
				is_on_ground = false;
			}
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				velocity.x += air_step * elapsed_time;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				velocity.x -= air_step * elapsed_time;

			velocity.x *= air_friction;

			velocity.x = std::min(velocity.x,  terminal_velocity);
			velocity.x = std::max(velocity.x, -terminal_velocity);
		}


		velocity.y = std::min(velocity.y,  terminal_velocity);
		velocity.y = std::max(velocity.y, -terminal_velocity);
		*/

		if (has_focus) {
			if (is_on_ground) {
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
					velocity.x += ground_step * elapsed_time;
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
					velocity.x -= ground_step * elapsed_time;

				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
					velocity.y += ground_step * elapsed_time;
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
					velocity.y -= ground_step * elapsed_time;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					velocity.y = -jump_velocity;
					is_on_ground = false;
				}
			}
			else {
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
					velocity.x += air_step * elapsed_time;
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
					velocity.x -= air_step * elapsed_time;

				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
					velocity.y += air_step * elapsed_time;
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
					velocity.y -= air_step * elapsed_time;
			}
		}

		velocity.x = velocity.x * (1.0f - elapsed_time * (is_on_ground ? ground_friction : air_friction));
		velocity.y += gravity * elapsed_time;
		
		velocity.x = std::min(velocity.x,  terminal_velocity_on_ground * elapsed_time);
		velocity.x = std::max(velocity.x, -terminal_velocity_on_ground * elapsed_time);

		velocity.y = std::min(velocity.y,  terminal_velocity * elapsed_time * 2.0f);
		velocity.y = std::max(velocity.y, -terminal_velocity * elapsed_time);
	}

	sf::Vector2f get_center() {
		return pos + size * 0.5f;
	}

	void spawn_randomly(Map &map) {
		pos = map.get_random_solid_position();
		pos.y -= size.y;
		pos.x += (std::ceil(size.x) - size.x) * 0.5f;
	}
};