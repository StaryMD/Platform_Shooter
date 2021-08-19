#pragma once

struct Camera {
	sf::Vector2f tile_count_per_side;
	float tilesize;

	Camera(sf::RenderWindow &window, const int tiles_per_sidescreen) {
		float smaller_screnside_length = window.getSize().y;
		tilesize = smaller_screnside_length / tiles_per_sidescreen;

		tile_count_per_side = { std::ceilf((float)tiles_per_sidescreen * window.getSize().x / window.getSize().y), float(tiles_per_sidescreen) };
	}

	sf::FloatRect get_visible_space(Player &player) {
		sf::Vector2f pos = player.pos;

		pos -= tile_count_per_side * 0.5f;

		return sf::FloatRect(pos.x, pos.y, tile_count_per_side.x, tile_count_per_side.y);
	}

};
