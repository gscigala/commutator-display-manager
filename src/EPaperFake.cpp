#include "EPaperFake.h"

#include <boost/log/trivial.hpp>

EPaperFake::EPaperFake(): EPaper()
{
	BOOST_LOG_TRIVIAL(info) << "EPaperFake: " << " created.";
}

EPaperFake::~EPaperFake()
{
	BOOST_LOG_TRIVIAL(info) << "EPaperFake: " << " destroyed.";
}

void EPaperFake::redraw(Display::Color** displayArray)
{
	/* To nothing */
}
