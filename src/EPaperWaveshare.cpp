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

	if (init() < 0) {
		throw std::runtime_error("Unable to initialize module!");
		return;
	}

	EPD_2IN15G_Clear(EPD_2IN15G_WHITE);

	BOOST_LOG_TRIVIAL(info) << "EPaperWaveshare: " << "EPaper initialized.";
}

EPaperWaveshare::~EPaperWaveshare()
{
	EPD_2IN15G_Clear(EPD_2IN15G_WHITE);
	
	exit();
	BOOST_LOG_TRIVIAL(info) << "EPaperWaveshare: " << " destroyed.";
}

int EPaperWaveshare::init()
{
	if(DEV_Module_Init() != 0) {
		BOOST_LOG_TRIVIAL(error) << "EPaperWaveshare: " << "Unable to initialize!";
		return -1;
	}

	EPD_2IN15G_Init();

	return 0;
}

void EPaperWaveshare::exit()
{
	EPD_2IN15G_Sleep();
	DEV_Delay_ms(2000); // required by Waveshare

	DEV_Module_Exit();	
}

void EPaperWaveshare::redraw(Display::Color** displayArray)
{
	BOOST_LOG_TRIVIAL(debug) << "EPaperWaveshare: redraw";

	if (init() < 0) {
		throw std::runtime_error("Unable to initialize module!");
		return;
	}
	
	UBYTE *image;
	UWORD Imagesize = ((EPD_2IN15G_WIDTH % 4 == 0)? (EPD_2IN15G_WIDTH / 4 ): (EPD_2IN15G_WIDTH / 4 + 1)) * EPD_2IN15G_HEIGHT;
	if((image = (UBYTE *)malloc(Imagesize)) == NULL) {
		BOOST_LOG_TRIVIAL(error) << "EPaperWaveshare: failed to allocate image memory";
		throw std::runtime_error("Unable to allocate image memory!");
		return;
	}

	Paint_NewImage(image, EPD_2IN15G_WIDTH, EPD_2IN15G_HEIGHT, 0, EPD_2IN15G_WHITE);
	Paint_SetScale(4);
	Paint_SelectImage(image);

	for(int y = 0; y < EPD_2IN15G_HEIGHT; y++) {
		for(int x = 0; x < EPD_2IN15G_WIDTH; x++) {
			EPaperWaveshare::Color color;

			switch (displayArray[x][y]) {
			case Display::Color::Black: {
				color = EPaperWaveshare::Color::Black;
				break;
			}
			case Display::Color::Yellow: {
				color = EPaperWaveshare::Color::Yellow;
				break;
			}
			case Display::Color::Red: {
				color = EPaperWaveshare::Color::Red;
				break;
			}
			case Display::Color::White:
			default: {
				color = EPaperWaveshare::Color::White;
				break;
			}
			}
			
			Paint_SetPixel(EPD_2IN15G_WIDTH-x,
				       y,
				       static_cast<UWORD>(color));
		}
	}

	EPD_2IN15G_Display(image);

	free(image);
	image = NULL;

	exit();
}
