#pragma once

#include "ship.hpp"
#include "galaxy.hpp"
#include "camera.hpp"

class Renderer {
private:
	Camera& camera;
	Ship& ship;
	World& world;
	CHAR_INFO*& screen;
public:

	Renderer(Camera& new_camera, World& new_world, Ship& new_ship, CHAR_INFO*& new_screen)
		: camera(new_camera), world(new_world), ship(new_ship) , screen(new_screen){
	}

	void render_screen() {
		for (int x = 0; x < camera.screen_width; x++) {
			for (int y = 0; y < camera.screen_height; y++) {

				if (  //logic for screen bounds
					(x + ship.ship_center_x < 0) ||
					(x + ship.ship_center_x > world.tile_map.size() - 1 ||
					(y + ship.ship_center_y < 0)||
					(y + ship.ship_center_y > world.tile_map.at(x).size() - 1)
					)) {
					screen[y * camera.screen_width + x].Attributes = FG_DARK_GRAY;
					screen[y * camera.screen_width + x].Char.UnicodeChar = L'#';
				}
				
				else {
					screen[y * camera.screen_width + x].Attributes = world.tile_map[x + ship.ship_center_x][y + ship.ship_center_y].color;
					screen[y * camera.screen_width + x].Char.UnicodeChar = world.tile_map[x + ship.ship_center_x][y + ship.ship_center_y].graphic;
				}
				//if ((x == 0) || (y==0) || (x = world.tile_map.size() - 1) || (y = world.tile_map.at(x).size()) ){
				//	screen[y * camera.screen_width + x].Attributes = FG_DARK_GRAY;
				//	screen[y * camera.screen_width + x].Char.UnicodeChar = L'#';
				//}
			}
		}
		ship.print_to_screen(screen, camera.screen_width, camera.screen_height, camera.mode);
	}
};