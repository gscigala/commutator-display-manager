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

	void printDisplayArray() const;

protected:
	char displayArray[Display::WIDGET_ROWS][Display::WIDGET_COLS];
};

#endif // WIDGET_H
