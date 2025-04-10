#ifndef EPAPERFAKE_H
#define EPAPERFAKE_H

#include "EPaper.h"

class EPaperFake: public EPaper {
public:
	EPaperFake();
	~EPaperFake();

	void redraw(Display::Color** displayArray);
};

#endif // EPAPERFAKE_H
