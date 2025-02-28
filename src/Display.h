#ifndef DISPLAY_H
#define DISPLAY_H

#include "../Fonts/fonts.h"

#include <string>

namespace Display {
	constexpr int WIDGET_ROWS = 79;
	constexpr int WIDGET_COLS = 147;

	constexpr int DISPLAY_ROWS = 160;
	constexpr int DISPLAY_COLS = 296;

	enum class WidgetPosition {
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};

	enum class Color {
		BLACK,
		WHITE,
		RED,
		YELLOW
	};
	
	std::string positionToString(WidgetPosition position);

	void drawChar(uint16_t x, uint16_t y, char ascii_char, sFONT* font, Display::Color textColor, Display::Color backgroundColor, Display::Color** displayArray);

	void drawString(uint16_t x, uint16_t y, std::string str, sFONT* font, Display::Color textcolor, Display::Color backgroundColor, Display::Color** displayArray);

	void drawImage(uint16_t x, uint16_t y,  Display::Color **image, int rows, int cols, Display::Color textcolor, Display::Color backgroundColor, Display::Color** displayArray);
}

#endif // DISPLAY_H
