#include "Display.h"

#include <boost/log/trivial.hpp>

namespace Display {

	std::string positionToString(WidgetPosition position) {
		switch (position) {
		case WidgetPosition::TOP_LEFT:
			return "TOP_LEFT";
		case WidgetPosition::TOP_RIGHT:
			return "TOP_RIGHT";
		case WidgetPosition::BOTTOM_LEFT:
			return "BOTTOM_LEFT";
		case WidgetPosition::BOTTOM_RIGHT:
			return "BOTTOM_RIGHT";
		default:
			return "UNKNOWN";
		}
	}
	
	void drawChar(uint16_t x, uint16_t y, char ascii_char, sFONT* font, Display::Color textColor, Display::Color backgroundColor, Display::Color** displayArray)
	{
		uint32_t char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
		const unsigned char *ptr = &font->table[char_offset];

		uint16_t page, column;

		if (y > Display::WIDGET_ROWS || x > Display::WIDGET_COLS) {
			BOOST_LOG_TRIVIAL(trace) << "input (" << x << "," << y << " exceeds the normal display range";
			return;
		}

		for (page = 0; page < font->Height; page ++ ) {
			for (column = 0; column < font->Width; column ++ ) {
				if (*ptr & (0x80 >> (column % 8))) {
					displayArray[y+page][x+column] = textColor;
				} else {
					displayArray[y+page][x+column] = backgroundColor;
				}

				//One pixel is 8 bits
				if (column % 8 == 7)
					ptr++;
			}// Write a line
			if (font->Width % 8 != 0)
				ptr++;
		}
	}

	void drawString(uint16_t x, uint16_t y, std::string str, sFONT* font, Display::Color textColor, Display::Color backgroundColor, Display::Color** displayArray)
	{
		const char* pString = str.c_str();
		
		if (y > Display::WIDGET_ROWS || x > Display::WIDGET_COLS) {
			BOOST_LOG_TRIVIAL(trace) << "input (" << x << "," << y << " exceeds the normal display range";
			return;
		}

		while (*pString != '\0') {

			drawChar(x, y, *pString, font, textColor, backgroundColor, displayArray);

			pString++;
			x += font->Width;
		}
	}

	void drawImage(uint16_t x, uint16_t y,  Display::Color** image, int rows, int cols, Display::Color textColor, Display::Color backgroundColor, Display::Color** displayArray)
	{
		if (y > Display::WIDGET_ROWS || x > Display::WIDGET_COLS) {
			BOOST_LOG_TRIVIAL(trace) << "input (" << x << "," << y << " exceeds the normal display range";
			return;
		}

		for (int page = 0; page < rows; page++) {
			for (int column = 0; column < cols; column++) {
				if (image[page][column] == Display::Color::BLACK)
					displayArray[y+page][x+column] = textColor;
				else
					displayArray[y+page][x+column] = backgroundColor;
			}
		}
	}
}
