#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "combat.hpp"
#include "crew.hpp"
#include "economy.hpp"
#include "events.hpp"
#include "factions.hpp"
#include "galaxy.hpp"
#include "color.hpp"
#include "ship.hpp"
#include "camera.hpp"
#include "renderer.hpp"
// Set the screen buffer size

Ship ship;

Camera camera;

World world;
// array[y * screen_width + x]

//void render() {
//}


/////debug flags:


int main() {

	world.build();
	CHAR_INFO* screen = new CHAR_INFO[camera.screen_width * camera.screen_height];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	Renderer renderer(camera, world, ship, screen);

	std::vector<std::vector<wchar_t>> ship_design = {
	{ L' ', L' ', L' ', L'#', L' ', L' ', L' ' },
	{ L' ', L' ', L'#', L'#', L'#', L' ', L' ' },
	{ L' ', L'#', L'#', L'#', L'#', L'#', L' ' },
	{ L'#', L'#', L'.', L'#', L'.', L'#', L'#' }
	};

	camera.mode = CHASE;
	
	ship.parts = ship.transpile_ship_map(ship_design, 3, 2, FG_GREEN);
	
	//std::thread render_thread(render);

	using clock = std::chrono::high_resolution_clock;
	auto lastTime = clock::now();
	auto currentTime = clock::now();
	
	bool running = 1;

	ship.update_camera(camera.relative_x_pos, camera.relative_y_pos);

	while (running) {

		currentTime = clock::now();
		std::chrono::duration<double> delta_time = currentTime - lastTime;
		lastTime = currentTime;

		// Then in your game loop, multiply by delta time (in seconds)
		if (GetAsyncKeyState('W') & 0x8000) {
			ship.ship_center_x += sin(ship.rotation_angle) * ship.movement_speed * delta_time.count();
			ship.ship_center_y -= cos(ship.rotation_angle) * ship.movement_speed * delta_time.count();
		}
		else if (GetAsyncKeyState('S') & 0x8000) {
			ship.ship_center_x -= sin(ship.rotation_angle) * ship.movement_speed * delta_time.count();
			ship.ship_center_y += cos(ship.rotation_angle) * ship.movement_speed * delta_time.count();
		}
		if (GetAsyncKeyState('Q') & 0x8000) {
			// Strafe left = 90 degrees counterclockwise from forward
			ship.ship_center_x -= cos(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
			ship.ship_center_y -= sin(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
		}
		else if (GetAsyncKeyState('E') & 0x8000) {
			// Strafe right = 90 degrees clockwise from forward
			ship.ship_center_x += cos(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
			ship.ship_center_y += sin(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
		}
		if (GetAsyncKeyState('A') & 0x8000) {
			ship.rotation_angle -= ship.rotation_speed * delta_time.count();
		}
		else if (GetAsyncKeyState('D') & 0x8000) {
			ship.rotation_angle += ship.rotation_speed * delta_time.count();
		}

		//if (GetAsyncKeyState('C') & 0x8000) {
		//	if (camera.mode == CHASE) {
		//		camera.mode = LOCKED;
		//	}
		//	else {
		//		camera.mode = CHASE;
		//	}
		//}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			running = 0;
		}

		//render stuff here:

		renderer.render_screen();
		//ship.print_to_screen(screen, camera.screen_width, camera.screen_height, camera.mode);
		COORD bufferSize = { camera.screen_width, camera.screen_height };
		COORD bufferCoord = { 0, 0 };
		SMALL_RECT writeRegion = { 0, 0, camera.screen_width - 1, camera.screen_height - 1 };
		WriteConsoleOutput(hConsole, screen, bufferSize, bufferCoord, &writeRegion);
		auto frame_time = std::chrono::high_resolution_clock::now() - currentTime;

	}

	return 0;
}