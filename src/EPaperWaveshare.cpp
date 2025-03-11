#include "EPaperWaveshare.h"

#include <boost/log/trivial.hpp>

extern "C" {
#include "EPD_2in15g.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
}

EPaperWaveshare::EPaperWaveshare(): EPaper()
{
	BOOST_LOG_TRIVIAL(info) << "EPaperWaveshare: " << "EPaper created.";

	if(DEV_Module_Init() != 0) {
		BOOST_LOG_TRIVIAL(error) << "EPaper: " << "Unable to initialize!";
		throw std::runtime_error("Unable to initialize module!");
		return;
	}

	EPD_2IN15G_Init();
	EPD_2IN15G_Clear(EPD_2IN15G_WHITE);

	BOOST_LOG_TRIVIAL(info) << "EPaperWaveshare: " << "EPaper initialized.";
}

EPaperWaveshare::~EPaperWaveshare()
{
	EPD_2IN15G_Sleep();
	DEV_Delay_ms(2000); // required by Waveshare

	DEV_Module_Exit();

	BOOST_LOG_TRIVIAL(info) << "EPaperWaveshare: " << " destroyed.";
}
