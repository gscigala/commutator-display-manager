#include "Widget.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

#include <iostream>

Widget::Widget()
{
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			displayArray[i][j] = 'w';
		}
	}
}

void Widget::printDisplayArray() const
{
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		std::string line;
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			line += displayArray[i][j];
		}
		std::cout << getName() << ": " << line << std::endl;
	}
}
