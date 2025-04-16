#include <windows.h>
#include <iostream>

#include "combat.hpp"
#include "crew.hpp"
#include "economy.hpp"
#include "events.hpp"
#include "factions.hpp"
#include "galaxy.hpp"
#include "color.hpp"
#include "ship.hpp"

short screen_width = 20;
short screen_height = 20;
CHAR_INFO* screen = new CHAR_INFO[screen_width * screen_height];
COORD bufferSize = { screen_width, screen_height };
COORD bufferCoord = { 0, 0 };
SMALL_RECT writeRegion = { 0, 0, screen_width - 1, screen_height - 1 };
HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
DWORD dwBytesWritten = 0;

// array[y * screen_width + x]

int main() {

	//////////////////////////////
	SetConsoleActiveScreenBuffer(hConsole);

	for (int x = 0; x < screen_width; x++) {
		for (int y = 0; y < screen_height; y++) {
			screen[y * screen_width + x].Attributes = FG_RED;
			screen[y * screen_width + x].Char.UnicodeChar = L'#';
		}	
	}

	screen[0].Attributes = FG_WHITE;
	screen[0].Char.UnicodeChar = L'O';
	
	while (1) {
		WriteConsoleOutput(hConsole, screen, bufferSize, bufferCoord, &writeRegion);
	}

	return 0;
}