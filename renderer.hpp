#pragma once

#include <windows.h>

enum COLOR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

class Renderer {
public:
	int my_screen_width;
	int my_screen_height;
	CHAR_INFO* screen;

	renderer(int screen_width, int screen_height) {
		my_screen_height = screen_height;
		my_screen_width = screen_width;
		CHAR_INFO* screen = new CHAR_INFO[screen_width * screen_height];
	}

	void set_char_graphic(int x, int y, wchar_t new_character) {
		screen[y + my_screen_width + x].Char.UnicodeChar = new_character;
	}

	void set_char_color(int x, int y, COLOR new_color) {
		screen[y + my_screen_width + x].Attributes = new_color;
	}

	void render(HANDLE& hConsole) {
		COORD bufferSize = { my_screen_width, my_screen_height };
		COORD bufferCoord = { 0, 0 };
		SMALL_RECT writeRegion = { 0, 0, my_screen_width - 1, my_screen_height - 1 };
		WriteConsoleOutput(hConsole, screen, bufferSize, bufferCoord, &writeRegion);
	}

};