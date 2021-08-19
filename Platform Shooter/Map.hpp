#pragma once

struct Map {
	sf::Vector2i size;
	uint16_t *grid;

	Map() {
		size.x = size.y = 0;
		grid = 0;
	}

	Map(int _size_x, int _size_y) {
		size.x = _size_x;
		size.y = _size_y;

		grid = new uint16_t[size.x * size.y];
		zerofy();
	}

	~Map() {
		if (grid)
			delete[] grid;
	}

	void zerofy() {
		uint16_t *end = grid_end();
		for (uint16_t *it = grid; it != end; it++)
			*it = 0;
	}

	void resize(int size_x, int size_y) {
		size = { size_x, size_y };

		if (grid)
			delete[] grid;
		grid = new uint16_t[size.x * size.y];

		zerofy();
	}

	bool is_inside(sf::Vector2i &pos) {
		return pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y;
	}

	bool is_inside(int x, int y) {
		return x >= 0 && y >= 0 && x < size.x && y < size.y;
	}

	uint16_t get_block(int x, int y) {
		return grid[y * size.x + x] & 0b0111111111111111ui16;
	}

	bool get_solidity(sf::Vector2i &pos) {
		if (!is_inside(pos.x, pos.y))
			return true;
		return grid[pos.y * size.x + pos.x] >> 14;
	}

	bool get_solidity(int x, int y) {
		return grid[y * size.x + x] >> 14;
	}

	bool is_solid(int x, int y) {
		if (!is_inside(x, y))
			return true;
		return grid[y * size.x + x] >> 14;
	}

	void load_from_file(const std::string &map_name) {
		if (!std::filesystem::exists("maps/" + map_name) || map_name == "") {
			std::cout << "Map doesn't exist\n";
			return;
		}

		std::ifstream fin("maps/" + map_name + "/map.dat", std::ios::binary | std::ios::in);
		int size_x = 0, size_y = 0;

		fin.read((char *)&size_x, sizeof(size_x));
		fin.read((char *)&size_y, sizeof(size_y));
		/* Useless data */ {
			uint16_t unique_tile_count = 0;
			fin.read((char *)&unique_tile_count, sizeof(unique_tile_count));
		}

		resize(size_x, size_y);
		fin.read((char *)grid, size_x * size_y * 2);
	}

	int get_solid_positions_count() {
		int count = 0;

		for (int y = 2; y < size.y; y++)
			for (int x = 0; x < size.x; x++)
				count += get_solidity(x, y) && !get_solidity(x, y - 1) && !get_solidity(x, y - 2);
		return count;
	}

	sf::Vector2f get_random_solid_position() {
		int random_position_num = rand() % get_solid_positions_count();

		for (int y = 2; y < size.y; y++)
			for (int x = 0; x < size.x; x++) {
				random_position_num -= get_solidity(x, y) && !get_solidity(x, y - 1) && !get_solidity(x, y - 2);

				if (!random_position_num)
					return sf::Vector2f(x, y);
			}
		return sf::Vector2f();
	}

	uint16_t *grid_end() {
		return grid + size.x * size.y;
	}
};