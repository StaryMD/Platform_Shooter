#pragma once

void draw_ontop(sf::Image &image, const std::string &path, int x, int y) {
	sf::Image temp;
	temp.loadFromFile(path);

	for (int i = 0; i < TILE_SIZE; i++)
		for (int j = 0; j < TILE_SIZE; j++)
			image.setPixel(j + x, i + y, temp.getPixel(j, i));
}

sf::Texture load_textures_from_map(const std::string &map_name) {
	const std::string map_tiles_path = "maps/" + map_name + "/tiles/";

	std::ifstream fin("maps/" + map_name + "/map.dat", std::ios::binary);

	int size_x = 0, size_y = 0;
	uint16_t tile_count = 0;
	fin.read((char *)&size_x, sizeof(size_x));
	fin.read((char *)&size_y, sizeof(size_y));
	fin.read((char *)&tile_count, sizeof(tile_count));

	fin.seekg(size_x * size_y * 2, std::ios::cur);

	sf::Image image;
	image.create(TILE_SIZE * tile_count, TILE_SIZE);

	for (auto i = 0; i < tile_count; i++) {
		uint16_t name_size = 0;
		char tile_name[101] = { 0 };

		fin.read((char *)&name_size, sizeof(name_size));
		fin.read(tile_name, name_size);

		std::string temp(tile_name);

		draw_ontop(image, map_tiles_path + temp, i * 16, 0);
	}

	sf::Texture texture;
	texture.loadFromImage(image);
	return texture;
}
