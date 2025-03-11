#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include "Widget.h"
#include "Display.h"
#include "EPaper.h"

#include <map>
#include <memory>
#include <thread>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using namespace boost::placeholders;

class Supervisor {
public:
	Supervisor(bool epaperFake);
	~Supervisor();

	int registerWidget(std::shared_ptr<Widget> widget, Display::WidgetPosition position);
	void start();

private:
	void generateDebugImage() const;
	void refreshWidget(Display::WidgetPosition position);
	void redraw();

	void newDataWidgetTopLeft();
	void newDataWidgetTopRight();
	void newDataWidgetBottomLeft();
	void newDataWidgetBottomRight();

	void refreshTimeout(const boost::system::error_code& error);

	void runIoService() {
		m_ioService.run();
	}

	std::unique_ptr<EPaper> m_epaper;

	std::map<Display::WidgetPosition, std::shared_ptr<Widget>> m_widgetMap;

	Display::Color** m_displayArray;

	boost::asio::io_service m_ioService;
	boost::asio::io_service::work m_work;
	std::thread m_ioThread;
	boost::asio::deadline_timer m_refreshTimer;

	/* minimal refresh rate */
	const int m_timeoutSec;

	bool m_newDisplayReady;

	int m_refreshCounter;
};

#endif // SUPERVISOR_H
