#ifndef EPAPER_H
#define EPAPER_H

#include "Display.h"

class EPaper {
public:
	EPaper();
	~EPaper();

	virtual void redraw(Display::Color** displayArray) = 0;
};

#endif // EPAPER_H
