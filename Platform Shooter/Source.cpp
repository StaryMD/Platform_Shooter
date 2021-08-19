#include "needed_headers.hpp"

#include "drawing_utils.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Camera.hpp"

int main() {
	srand(time(NULL));
	// CONSTANTS

	std::string map_name = "cave";
	const int tiles_per_sidescreen = 15;

	// END

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Platform Shooter", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true); 
	window.setMouseCursorVisible(false);

	Map map;
	map.load_from_file(map_name);

	Player player(map);
	Camera camera(window, tiles_per_sidescreen);

	sf::Texture textures = load_textures_from_map(map_name);
	sf::Sprite sprite;
	sprite.setTexture(textures);
	sprite.setScale({ camera.tilesize / 16.0f, camera.tilesize / 16.0f });

	sf::Event event;
	std::chrono::system_clock::time_point last_frame_start = std::chrono::system_clock::now();
	while (window.isOpen()) {
		std::chrono::duration<float> elapsed_time_duration = std::chrono::system_clock::now() - last_frame_start;
		float elapsed_time = elapsed_time_duration.count();
		last_frame_start = std::chrono::system_clock::now();

		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
			}
		}

		player.update(elapsed_time, map, window.hasFocus());

		//window.clear();

		sf::FloatRect visible_space = camera.get_visible_space(player);
		int tiles_count_x = visible_space.width,
			tiles_count_y = visible_space.height;

		visible_space.left = std::max(0.1f, visible_space.left);
		visible_space.top  = std::max(0.1f, visible_space.top);
		visible_space.left = std::min(map.size.x - visible_space.width, visible_space.left);
		visible_space.top  = std::min(map.size.y - visible_space.height, visible_space.top);

		sf::Vector2f tile_offset = sf::Vector2f(std::fmodf(visible_space.left, 1.0f), std::fmodf(visible_space.top, 1.0f));
		sf::Vector2f pos = sf::Vector2f(-tile_offset.x * camera.tilesize, -tile_offset.y * camera.tilesize);
		int map_offset_x = visible_space.left, map_offset_y = visible_space.top;
		int lastblock_id = -1;

		for (int i = 0; i <= tiles_count_y; i++) {
			for (int j = 0; j <= tiles_count_x; j++) {
				if (map.is_inside(j + map_offset_x, i + map_offset_y)) {
					//sf::Vector2f pos = sf::Vector2f((j - tile_offset.x) * camera.tilesize, (i - tile_offset.y) * camera.tilesize);
					int value = map.get_block(j + map_offset_x, i + map_offset_y);

					if (lastblock_id != value)
						sprite.setTextureRect(sf::IntRect((lastblock_id = value) * 16, 0, 16, 16));
					sprite.setPosition(pos);

					window.draw(sprite);
				}
				pos.x += camera.tilesize;
			}
			pos.y += camera.tilesize;
			pos.x = -tile_offset.x * camera.tilesize;
		}

		{
			sf::RectangleShape shape;

			shape.setPosition((player.pos - sf::Vector2f(visible_space.left, visible_space.top)) * camera.tilesize);
			shape.setFillColor(sf::Color::Green);
			shape.setSize({ player.size.x * camera.tilesize , player.size.y * camera.tilesize });
			window.draw(shape);
		}
		window.display();
	}
	
	return 0;
}
