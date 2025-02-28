#ifndef WIDGET_H
#define WIDGET_H

#include "Commutator.h"
#include "Display.h"

#include <memory>
#include <string>

#include <boost/signals2.hpp>

class Widget {
public:
	Widget();
	~Widget();

	virtual std::string getName() const = 0;

	void generateDebugImage() const;
	int loadImage(std::string imagePath, Display::Color** const array, int rows, int cols) const;

	const Display::Color* const* getDisplayArray() const {
		return m_displayArray;
	}

	boost::signals2::signal<void()> newData;

protected:
	Display::Color** m_displayArray;
};

#endif // WIDGET_H
