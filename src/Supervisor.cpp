#include "Supervisor.h"

#include <boost/log/trivial.hpp>

#include <opencv2/opencv.hpp>

Supervisor::Supervisor():
	m_ioService(),
	m_work(m_ioService),
	m_ioThread(&Supervisor::runIoService, this),
	m_refreshTimer(m_ioService),
	m_timeoutSec(180),
	m_newDisplayReady(false),
	m_refreshCounter(0)
{
	BOOST_LOG_TRIVIAL(info) << "Supervisor: " << "Supervisor created.";

	m_displayArray = new Display::Color*[Display::DISPLAY_ROWS];
	for (int i = 0; i < Display::DISPLAY_ROWS; ++i) {
	        m_displayArray[i] = new Display::Color[Display::DISPLAY_COLS];
	}

	for (int i = 0; i < Display::DISPLAY_ROWS; ++i) {
		for (int j = 0; j < Display::DISPLAY_COLS; ++j) {
			m_displayArray[i][j] = Display::Color::WHITE;
		}
	}

	/* central draw separator */
	for (int i = 0; i < Display::DISPLAY_ROWS; ++i) {
		for (int j = 0; j < Display::DISPLAY_COLS; ++j) {
			if ((i == Display::DISPLAY_ROWS/2)
			    || (i == Display::DISPLAY_ROWS/2 - 1))
				m_displayArray[i][j] = Display::Color::BLACK;
			if ((j == Display::DISPLAY_COLS/2)
				|| (j == Display::DISPLAY_COLS/2 - 1))
				m_displayArray[i][j] = Display::Color::BLACK;
		}
	}
}

Supervisor::~Supervisor()
{
	BOOST_LOG_TRIVIAL(trace) << "Supervisor: destructor!";

	m_refreshTimer.cancel();

	m_ioService.stop();
	if (m_ioThread.joinable())
            m_ioThread.join();

	for (int i = 0; i < Display::DISPLAY_ROWS; ++i) {
		delete[] m_displayArray[i];
	}
	delete[] m_displayArray;
}

int Supervisor::registerWidget(std::shared_ptr<Widget> widget, Display::WidgetPosition position)
{
	m_widgetMap[position] = widget;
	BOOST_LOG_TRIVIAL(debug) << "Supervisor: " << widget->getName() << " registered at position: " << Display::positionToString(position);

	if (position == Display::WidgetPosition::TOP_LEFT)
		widget->newData.connect(boost::bind(&Supervisor::newDataWidgetTopLeft, this));
	if (position == Display::WidgetPosition::TOP_RIGHT)
		widget->newData.connect(boost::bind(&Supervisor::newDataWidgetTopRight, this));
	if (position == Display::WidgetPosition::BOTTOM_LEFT)
		widget->newData.connect(boost::bind(&Supervisor::newDataWidgetBottomLeft, this));
	if (position == Display::WidgetPosition::BOTTOM_RIGHT)
		widget->newData.connect(boost::bind(&Supervisor::newDataWidgetBottomRight, this));
	
	return 0;
}

void Supervisor::start()
{
	refreshWidget(Display::WidgetPosition::TOP_LEFT);
	refreshWidget(Display::WidgetPosition::TOP_RIGHT);
	refreshWidget(Display::WidgetPosition::BOTTOM_LEFT);
	refreshWidget(Display::WidgetPosition::BOTTOM_RIGHT);

	redraw();
}

void Supervisor::generateDebugImage() const {
	cv::Mat image(Display::DISPLAY_ROWS, Display::DISPLAY_COLS, CV_8UC3, cv::Scalar(255, 255, 255));

	for (int i = 0; i < Display::DISPLAY_ROWS; ++i) {
		for (int j = 0; j < Display::DISPLAY_COLS; ++j) {
			cv::Scalar color;
			switch (m_displayArray[i][j]) {
			case Display::Color::BLACK:
				color = cv::Scalar(3, 11, 18);
				break;
			case Display::Color::RED:
				color = cv::Scalar(11, 25, 161);
				break;
			case Display::Color::YELLOW:
				color = cv::Scalar(14, 180, 255);
				break;
			case Display::Color::WHITE:
			default:
				color = cv::Scalar(208, 203, 203);
				break;
			}
			image.at<cv::Vec3b>(i, j) = cv::Vec3b(color[0], color[1], color[2]);
		}
	}

	cv::imwrite("/tmp/Supervisor.png", image);
}

void Supervisor::refreshWidget(Display::WidgetPosition position)
{
	const int xOffsetTopLeft = 0;
	const int yOffsetTopLeft = 0;
	
	const int xOffsetTopRight = 149;
	const int yOffsetTopRight = 0;

	const int xOffsetBottomLeft = 0;
	const int yOffsetBottomLeft = 81;
	
	const int xOffsetBottomRight = 149;
	const int yOffsetBottomRight = 81;
	
	int xOffset = 0;
	int yOffset = 0;

	std::shared_ptr<Widget> widget = m_widgetMap[position];

	BOOST_LOG_TRIVIAL(debug) << "Supervisor: " << "refresh widget " << positionToString(position);
	
	if (widget != nullptr) {
		const Display::Color* const* widgetArray = widget->getDisplayArray();

		if (position == Display::WidgetPosition::TOP_LEFT) {
		        xOffset = xOffsetTopLeft;
		        yOffset = yOffsetTopLeft;
		}

		if (position == Display::WidgetPosition::TOP_RIGHT) {
		        xOffset = xOffsetTopRight;
			yOffset = yOffsetTopRight;
		}

		if (position == Display::WidgetPosition::BOTTOM_LEFT) {
		        xOffset = xOffsetBottomLeft;
		        yOffset = yOffsetBottomLeft;
		}

		if (position == Display::WidgetPosition::BOTTOM_RIGHT) {
		        xOffset = xOffsetBottomRight;
		        yOffset = yOffsetBottomRight;
		}

		for (int page = 0; page < Display::WIDGET_ROWS; page++) {
			for (int column = 0; column < Display::WIDGET_COLS; column++) {
				m_displayArray[page+yOffset][column+xOffset] = widgetArray[page][column];
			}
		}
	}
}

void Supervisor::newDataWidgetTopLeft()
{
	BOOST_LOG_TRIVIAL(trace) << "Supervisor: " << "newDataWidgetTopLeft";

	refreshWidget(Display::WidgetPosition::TOP_LEFT);

	if (m_refreshTimer.expires_at() > boost::asio::deadline_timer::traits_type::now()) {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: wait for next display refresh";
		m_newDisplayReady = true;
	} else {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: redraw now!";
		redraw();
	}
}

void Supervisor::newDataWidgetTopRight()
{
	BOOST_LOG_TRIVIAL(trace) << "Supervisor: " << "newDataWidgetTopRight";

	refreshWidget(Display::WidgetPosition::TOP_RIGHT);

	if (m_refreshTimer.expires_at() > boost::asio::deadline_timer::traits_type::now()) {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: wait for next display refresh";
		m_newDisplayReady = true;
	} else {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: redraw now!";
		redraw();
	}
}

void Supervisor::newDataWidgetBottomLeft()
{
	BOOST_LOG_TRIVIAL(trace) << "Supervisor: " << "newDataWidgetBottomLeft";

	refreshWidget(Display::WidgetPosition::BOTTOM_LEFT);

	if (m_refreshTimer.expires_at() > boost::asio::deadline_timer::traits_type::now()) {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: wait for next display refresh";
		m_newDisplayReady = true;
	} else {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: redraw now!";
		redraw();
	}
}


void Supervisor::newDataWidgetBottomRight()
{
	BOOST_LOG_TRIVIAL(trace) << "Supervisor: " << "newDataWidgetBottomRight";

	refreshWidget(Display::WidgetPosition::BOTTOM_RIGHT);

	if (m_refreshTimer.expires_at() > boost::asio::deadline_timer::traits_type::now()) {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: wait for next display refresh";
		m_newDisplayReady = true;
	} else {
		BOOST_LOG_TRIVIAL(trace) << "Supervisor: redraw now!";
		redraw();
	}
}

void Supervisor::refreshTimeout(const boost::system::error_code& error)
{
	if (!error && m_newDisplayReady) {
		BOOST_LOG_TRIVIAL(debug) << "Supervisor: timeout, redraw needed";
		m_newDisplayReady = false;
		redraw();
	} else {
		BOOST_LOG_TRIVIAL(debug) << "Supervisor: timeout, no redraw needed";
	}
}

void Supervisor::redraw()
{
	BOOST_LOG_TRIVIAL(debug) << "Supervisor: redraw";

	m_refreshCounter++;
	BOOST_LOG_TRIVIAL(trace) << "Supervisor: redraw number " << m_refreshCounter;
	
	/* WIP code display here */

	generateDebugImage();

	BOOST_LOG_TRIVIAL(trace) << "Supervisor: redraw, start timer";
	m_refreshTimer.expires_from_now(boost::posix_time::seconds(m_timeoutSec));
	m_refreshTimer.async_wait(boost::bind(&Supervisor::refreshTimeout, this, boost::asio::placeholders::error));
}
