#pragma once

#include <vector>
#include <random>

#include "color.hpp"

struct Tile {
	wchar_t graphic;
	COLOR color;
};

class World {
private:
	std::vector<Tile> stars = {
		Tile{L'.', FG_RED},
		Tile{L'.', FG_GREEN},
		Tile{L'.', FG_BLUE},
		Tile{L'.', FG_YELLOW},
		Tile{L'.', FG_WHITE}
	};
public:
	std::vector<std::vector<Tile>> tile_map;


	void build() {
		std::random_device rd;
		std::mt19937 rng(rd()); // Mersenne Twister engine
		std::uniform_int_distribution<int> stars_dist(0, stars.size() - 1);
		std::uniform_int_distribution<int> tf_dist(0, 1);
		std::uniform_int_distribution<int> hundred_dist(0, 100);
		std::uniform_int_distribution<int> thousand_dist(0, 1000);


		for (int x = 0; x < 1000; x++) {
			tile_map.push_back(std::vector<Tile>());
			for (int y = 0; y < 1000; y++) {
				if (thousand_dist(rng) == 1) {
					tile_map[x].push_back(stars[stars_dist(rng)]);
				}
				else {
					tile_map[x].push_back(Tile{ L'#', FG_BLACK });
				}
			}
		}
	}
};