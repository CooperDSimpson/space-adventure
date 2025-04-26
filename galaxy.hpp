//#pragma once
//
//#include <vector>
//#include <random>
//
//#include "color.hpp"
//
//struct Tile {
//	wchar_t graphic;
//	COLOR color;
//};
//
//class World {
//private:
//	std::vector<Tile> stars = {
//		Tile{L'.', FG_RED},
//		Tile{L'.', FG_GREEN},
//		Tile{L'.', FG_BLUE},
//		Tile{L'.', FG_YELLOW},
//		Tile{L'.', FG_WHITE}
//	};
//public:
//	std::vector<std::vector<Tile>> tile_map;
//
//
//	void build() {
//		std::random_device rd;
//		std::mt19937 rng(rd()); // Mersenne Twister engine
//		std::uniform_int_distribution<int> stars_dist(0, stars.size() - 1);
//		std::uniform_int_distribution<int> tf_dist(0, 1);
//		std::uniform_int_distribution<int> hundred_dist(0, 100);
//		std::uniform_int_distribution<int> thousand_dist(0, 1000);
//
//
//		for (int x = 0; x < 1000; x++) {
//			tile_map.push_back(std::vector<Tile>());
//			for (int y = 0; y < 1000; y++) {
//				if (thousand_dist(rng) == 1) {
//					tile_map[x].push_back(stars[stars_dist(rng)]);
//				}
//				else {
//					tile_map[x].push_back(Tile{ L'#', FG_BLACK });
//				}
//			}
//		}
//	}
//};

// galaxy.hpp - Updated for infinite world

#pragma once

#include <vector>
#include <random>
#include <unordered_map>
#include <functional>
#include <cstdint>
#include "color.hpp"

// Chunk coordinates
struct ChunkCoord {
    int x;
    int y;

    bool operator==(const ChunkCoord& other) const {
        return x == other.x && y == other.y;
    }
};

// Custom hash function for ChunkCoord
struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
    }
};

struct Tile {
    wchar_t graphic;
    COLOR color;
};

// Size of each chunk
const int CHUNK_SIZE = 64;

class Chunk {
public:
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    bool is_generated = false;
};

class World {
private:
    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> chunks;
    std::vector<Tile> star_templates = {
        Tile{L'.', FG_RED},
        Tile{L'.', FG_GREEN},
        Tile{L'.', FG_BLUE},
        Tile{L'.', FG_YELLOW},
        Tile{L'.', FG_WHITE},
        // More variety
        Tile{L'*', FG_WHITE},
        Tile{L'*', FG_YELLOW},
        Tile{L'+', FG_CYAN},
        Tile{L'o', FG_MAGENTA}
    };

    // Star density (lower = more sparse)
    int star_rarity = 800;

    // Use the chunk coordinates as seed for deterministic generation
    Chunk generate_chunk(int chunk_x, int chunk_y) {
        Chunk new_chunk;

        // Create a seed from chunk coordinates
        uint64_t seed = (static_cast<uint64_t>(chunk_x) << 32) | static_cast<uint64_t>(chunk_y & 0xFFFFFFFF);
        std::mt19937_64 rng(seed);
        std::uniform_int_distribution<int> stars_dist(0, star_templates.size() - 1);
        std::uniform_int_distribution<int> rarity_dist(0, star_rarity);

        // Generate the chunk content
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                if (rarity_dist(rng) == 1) {
                    new_chunk.tiles[x][y] = star_templates[stars_dist(rng)];
                }
                else {
                    new_chunk.tiles[x][y] = Tile{ L' ', FG_BLACK }; // Space background
                }
            }
        }

        new_chunk.is_generated = true;
        return new_chunk;
    }

public:
    // Initial setup - no longer builds the entire world at once
    void build() {
        // Just initialize the RNG and any global world properties
        // Individual chunks will be generated on demand
    }

    // Get a tile at any world coordinate
    Tile get_tile(int world_x, int world_y) {
        // Convert world coordinates to chunk coordinates
        int chunk_x = world_x >= 0 ? world_x / CHUNK_SIZE : (world_x - CHUNK_SIZE + 1) / CHUNK_SIZE;
        int chunk_y = world_y >= 0 ? world_y / CHUNK_SIZE : (world_y - CHUNK_SIZE + 1) / CHUNK_SIZE;

        // Calculate local coordinates within the chunk
        int local_x = world_x >= 0 ? world_x % CHUNK_SIZE : (CHUNK_SIZE + world_x % CHUNK_SIZE) % CHUNK_SIZE;
        int local_y = world_y >= 0 ? world_y % CHUNK_SIZE : (CHUNK_SIZE + world_y % CHUNK_SIZE) % CHUNK_SIZE;

        ChunkCoord coord{ chunk_x, chunk_y };

        // Check if chunk exists, generate if not
        if (chunks.find(coord) == chunks.end()) {
            chunks[coord] = generate_chunk(chunk_x, chunk_y);
        }

        return chunks[coord].tiles[local_x][local_y];
    }

    // Set a tile at any world coordinate (for modifications)
    void set_tile(int world_x, int world_y, const Tile& tile) {
        int chunk_x = world_x >= 0 ? world_x / CHUNK_SIZE : (world_x - CHUNK_SIZE + 1) / CHUNK_SIZE;
        int chunk_y = world_y >= 0 ? world_y / CHUNK_SIZE : (world_y - CHUNK_SIZE + 1) / CHUNK_SIZE;

        int local_x = world_x >= 0 ? world_x % CHUNK_SIZE : (CHUNK_SIZE + world_x % CHUNK_SIZE) % CHUNK_SIZE;
        int local_y = world_y >= 0 ? world_y % CHUNK_SIZE : (CHUNK_SIZE + world_y % CHUNK_SIZE) % CHUNK_SIZE;

        ChunkCoord coord{ chunk_x, chunk_y };

        // Make sure the chunk exists
        if (chunks.find(coord) == chunks.end()) {
            chunks[coord] = generate_chunk(chunk_x, chunk_y);
        }

        chunks[coord].tiles[local_x][local_y] = tile;
    }

    // For memory optimization - unload chunks that are far from the player
    void unload_distant_chunks(int center_world_x, int center_world_y, int radius_chunks) {
        int center_chunk_x = center_world_x / CHUNK_SIZE;
        int center_chunk_y = center_world_y / CHUNK_SIZE;

        // Get a list of chunks to remove
        std::vector<ChunkCoord> to_remove;

        for (const auto& chunk_pair : chunks) {
            const ChunkCoord& coord = chunk_pair.first;
            int dx = abs(coord.x - center_chunk_x);
            int dy = abs(coord.y - center_chunk_y);

            // If chunk is outside radius, mark for removal
            if (dx > radius_chunks || dy > radius_chunks) {
                to_remove.push_back(coord);
            }
        }

        // Remove distant chunks
        for (const auto& coord : to_remove) {
            chunks.erase(coord);
        }
    }

    // Adds special objects like nebulas, black holes, etc. to a region
    void create_special_region(int center_x, int center_y, int radius, int type) {
        // Types can be: 0=nebula, 1=asteroid field, 2=black hole, etc.
        // Implementation depends on your game mechanics

        // Example: Create a nebula (colored space)
        if (type == 0) {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> color_dist(0, 3);

            COLOR nebula_colors[] = { FG_DARK_MAGENTA, FG_DARK_BLUE, FG_DARK_CYAN, FG_DARK_RED };
            COLOR chosen_color = nebula_colors[color_dist(rng)];

            for (int x = center_x - radius; x <= center_x + radius; x++) {
                for (int y = center_y - radius; y <= center_y + radius; y++) {
                    // Only within the circular area
                    int dx = x - center_x;
                    int dy = y - center_y;
                    if (dx * dx + dy * dy <= radius * radius) {
                        // Get existing tile
                        Tile current = get_tile(x, y);

                        // If it's empty space, add nebula background
                        if (current.graphic == L' ') {
                            // 50% chance to place nebula tile
                            if (rng() % 2 == 0) {
                                set_tile(x, y, Tile{ L'·', chosen_color });
                            }
                        }
                    }
                }
            }
        }
    }
};