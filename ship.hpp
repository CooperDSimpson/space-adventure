#pragma once

#include <vector>
#include "color.hpp"
#include "camera.hpp"


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
private:

	double camera_relative_x = 0;
	double camera_relative_y = 0;

public:

	double velocity_x = 0;
	double velocity_y = 0;

	double ship_center_x = 5;
	double ship_center_y = 5;

	double rotation_angle = 0.0;  // radians

	float movement_speed = 60.0f; // Units per second
	float strafe_speed = 20.0f;   // Units per second
	float rotation_speed = 2.0f;  // Radians per second

	std::vector<Ship_Part> parts{};

	void update_camera(double new_camera_relative_x, double new_camera_relative_y) {
		camera_relative_x = new_camera_relative_x;
		camera_relative_y = new_camera_relative_y;
	}

	void teleport(double x, double y) {
		ship_center_x = x;
		ship_center_y = y;
	}

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

					if (rel_x == 0 && rel_y == 0) {
						color = BG_RED;
					}


					result.emplace_back(ch, color, rel_x, rel_y);
				}
			}
		}

		return result;
	}

	void update_ship_shit(double delta_time) {
		ship_center_x += velocity_x * delta_time;
		ship_center_y += velocity_y * delta_time;

		velocity_x = velocity_x * 0.9999;
		velocity_y = velocity_y * 0.9999;
	}


	void print_to_screen(CHAR_INFO*& screen, short screen_width, short screen_height, CAMERA_MODE c_mode) {
		double cos_theta = cos(rotation_angle);
		double sin_theta = sin(rotation_angle);

		for (const Ship_Part& sp : parts) {
			// Rotate around center

			double rotated_x = sp.x_loc * cos_theta - sp.y_loc * sin_theta;
			double rotated_y = sp.x_loc * sin_theta + sp.y_loc * cos_theta;


			switch (c_mode) {
			case LOCKED: {
				camera_relative_x = ship_center_x;
				camera_relative_y = ship_center_y;
				break;
			}
			case CHASE: {
				camera_relative_x = screen_width / 2;
				camera_relative_y = screen_height / 2;
				break;
		 	}
			}


			// Translate to screen
			int screen_x = static_cast<int>(round(rotated_x)) + static_cast<int>(camera_relative_x);
			int screen_y = static_cast<int>(round(rotated_y)) + static_cast<int>(camera_relative_y);

			// Bounds check
			if (screen_x >= 0 && screen_x < screen_width && screen_y >= 0 && screen_y < screen_height) {
				screen[screen_y * screen_width + screen_x].Attributes = sp.color;
				screen[screen_y * screen_width + screen_x].Char.UnicodeChar = sp.graphic;
			}
		}
	}
};