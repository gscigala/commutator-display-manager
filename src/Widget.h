#ifndef WIDGET_H
#define WIDGET_H

#include "Commutator.h"
#include "Display.h"

#include <memory>
#include <string>

class Widget {
public:
	Widget();

	virtual std::string getName() const = 0;

	void generateDebugImage() const;
	int loadImage(std::string imagePath, Display::Color** const array, int rows, int cols) const;

protected:
	Display::Color displayArray[Display::WIDGET_ROWS][Display::WIDGET_COLS];
};

#endif // WIDGET_H
