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

int screen_width = 400;
int screen_height = 400;

// Set the screen buffer size

Ship ship;

// array[y * screen_width + x]

//void render() {
//}

int main() {
	CHAR_INFO* screen = new CHAR_INFO[screen_width * screen_height];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	std::vector<std::vector<wchar_t>> ship_design = {
	{ L' ', L' ', L' ', L'#', L' ', L' ', L' ' },
	{ L' ', L' ', L'#', L'#', L'#', L' ', L' ' },
	{ L' ', L'#', L'#', L'#', L'#', L'#', L' ' },
	{ L'#', L'#', L'.', L'#', L'.', L'#', L'#' }
	};
	
	ship.parts = ship.transpile_ship_map(ship_design, 3, 2, FG_GREEN);
	
	//std::thread render_thread(render);

	using clock = std::chrono::high_resolution_clock;
	std::chrono::milliseconds target_frame_time(1000 / 240);  // 240 FPS means 1000 ms / 240 = 4.167 ms
	auto lastTime = clock::now();
	auto currentTime = clock::now();
	
	bool running = 1;

	while (running) {

		currentTime = clock::now();
		const std::chrono::duration<double> target_frame_duration(1.0 / 240.0); // Target frame duration for 240 FPS
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
		if (GetAsyncKeyState('A') & 0x8000) {
			// Strafe left = 90 degrees counterclockwise from forward
			ship.ship_center_x -= cos(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
			ship.ship_center_y -= sin(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
		}
		else if (GetAsyncKeyState('D') & 0x8000) {
			// Strafe right = 90 degrees clockwise from forward
			ship.ship_center_x += cos(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
			ship.ship_center_y += sin(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
		}
		if (GetAsyncKeyState('Q') & 0x8000) {
			ship.rotation_angle -= ship.rotation_speed * delta_time.count();
		}
		else if (GetAsyncKeyState('E') & 0x8000) {
			ship.rotation_angle += ship.rotation_speed * delta_time.count();
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			running = 0;
		}

		for (int x = 0; x < screen_width; x++) {
			for (int y = 0; y < screen_height; y++) {
				screen[y * screen_width + x].Attributes = FG_BLACK;
				screen[y * screen_width + x].Char.UnicodeChar = L'#';
			}
		}

		//render stuff here:
		ship.print_to_screen(screen, screen_width, screen_height);
		COORD bufferSize = { screen_width, screen_height };
		COORD bufferCoord = { 0, 0 };
		SMALL_RECT writeRegion = { 0, 0, screen_width - 1, screen_height - 1 };
		WriteConsoleOutput(hConsole, screen, bufferSize, bufferCoord, &writeRegion);
		auto frame_time = std::chrono::high_resolution_clock::now() - currentTime;

	}

	return 0;
}