#ifndef EPAPERWAVESHARE_H
#define EPAPERWAVESHARE_H

#include "EPaper.h"

class EPaperWaveshare: public EPaper {
	enum class Color { // In order corresponding to epaper EPD 2in15g
		Black = 0,
		White = 1,
		Yellow = 2,
		Red = 3
	};
public:
	EPaperWaveshare();
	~EPaperWaveshare();

	int init();
	void exit();
	void redraw(Display::Color** displayArray);
};

#endif // EPAPERWAVESHARE_H
