#ifndef DISPLAY_H
#define DISPLAY_H

#include "../Fonts/fonts.h"

#include <string>

namespace Display {
	constexpr int WIDGET_ROWS = 70;
	constexpr int WIDGET_COLS = 130;

	enum class Color {
		BLACK,
		WHITE,
		RED,
		YELLOW
	};

	void drawChar(uint16_t x, uint16_t y, char ascii_char, sFONT* font, Display::Color textColor, Display::Color backgroundColor, Display::Color displayArray[Display::WIDGET_ROWS][Display::WIDGET_COLS]);
	void drawString(uint16_t x, uint16_t y, std::string str, sFONT* font, Display::Color textcolor, Display::Color backgroundColor, Display::Color displayArray[Display::WIDGET_ROWS][Display::WIDGET_COLS]);
	void drawImage(uint16_t x, uint16_t y,  Display::Color **image, int rows, int cols, Display::Color textcolor, Display::Color backgroundColor, Display::Color displayArray[Display::WIDGET_ROWS][Display::WIDGET_COLS]);
}

#endif // DISPLAY_H
