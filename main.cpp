#include <windows.h>

#include "combat.hpp"
#include "crew.hpp"
#include "economy.hpp"
#include "events.hpp"
#include "factions.hpp"
#include "galaxy.hpp"
#include "color.hpp"
#include "ship.hpp"

int screen_width = 50;
int screen_height = 50;

CHAR_INFO* screen = new CHAR_INFO[screen_width * screen_height];


//set unicode character
void set_char(int x, int y, wchar_t new_character) {
	screen[y * screen_width + x].Char.UnicodeChar = new_character;
}

//set character color
void set_char(int x, int y, COLOR new_color) {
	screen[y * screen_width + x].Attributes = new_color;
}

//set both
void set_char(int x, int y, wchar_t new_character, COLOR new_color) {
	int idx = y * screen_width + x;
	screen[idx].Char.UnicodeChar = new_character;
	screen[idx].Attributes = new_color;
}

int main() {

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);

	DWORD dwBytesWritten = 0;

	set_char(0,0, L'O');
	set_char(0,0,FG_WHITE);
	
	while (1) {
		COORD bufferSize = { screen_width, screen_height };
		COORD bufferCoord = { 0, 0 };
		SMALL_RECT writeRegion = { 0, 0, screen_width - 1, screen_height - 1 };
		WriteConsoleOutput(hConsole, screen, bufferSize, bufferCoord, &writeRegion);
	}

	return 0;
}