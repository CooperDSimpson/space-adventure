#pragma once

#include <vector>
#include "color.hpp"

class Ship_Part {
public:
	wchar_t graphic;
	COLOR color;
	int x_loc;
	int y_loc;
	Ship_Part(wchar_t new_graphic, COLOR new_color, int new_x_loc, int new_y_loc) {
		this->graphic = new_graphic;
		this->color = new_color;
		this->x_loc = new_x_loc;
		this->y_loc = new_y_loc;
	}
};

class Ship {
public:

	double ship_center_x = 5;
	double ship_center_y = 5;

	double rotation_angle = 0.0;  // radians

	float movement_speed = 60.0f; // Units per second
	float strafe_speed = 20.0f;   // Units per second
	float rotation_speed = 2.0f;  // Radians per second

	std::vector<Ship_Part> parts{};

	std::vector<Ship_Part> transpile_ship_map(
		const std::vector<std::vector<wchar_t>>& map,
		int anchor_x,
		int anchor_y,
		COLOR color = FG_WHITE,
		wchar_t empty_char = L' '
	) {
		std::vector<Ship_Part> result;

		for (int y = 0; y < map.size(); ++y) {
			for (int x = 0; x < map[y].size(); ++x) {
				wchar_t ch = map[y][x];

				switch (ch) {
				case L'.': {color = FG_DARK_CYAN; break;}
				case L'#': { color = FG_GREEN; break; }
				}

				if (ch != empty_char) {
					// Relative to the anchor point
					int rel_x = x - anchor_x;
					int rel_y = y - anchor_y;
					result.emplace_back(ch, color, rel_x, rel_y);
				}
			}
		}

		return result;
	}


	void print_to_screen(CHAR_INFO*& screen, short screen_width, short screen_height) {
		double cos_theta = cos(rotation_angle);
		double sin_theta = sin(rotation_angle);

		for (const Ship_Part& sp : parts) {
			// Rotate around center
			double rotated_x = sp.x_loc * cos_theta - sp.y_loc * sin_theta;
			double rotated_y = sp.x_loc * sin_theta + sp.y_loc * cos_theta;

			// Translate to screen
			int screen_x = static_cast<int>(round(rotated_x)) + static_cast<int>(ship_center_x);
			int screen_y = static_cast<int>(round(rotated_y)) + static_cast<int>(ship_center_y);

			// Bounds check
			if (screen_x >= 0 && screen_x < screen_width && screen_y >= 0 && screen_y < screen_height) {
				screen[screen_y * screen_width + screen_x].Attributes = sp.color;
				screen[screen_y * screen_width + screen_x].Char.UnicodeChar = sp.graphic;
			}
		}
	}
};