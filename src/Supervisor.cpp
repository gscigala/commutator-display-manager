#include "Supervisor.h"

#include <boost/log/trivial.hpp>

Supervisor::Supervisor() {
	BOOST_LOG_TRIVIAL(info) << "Supervisor: " << "Supervisor created.";
}

std::string Supervisor::positionToString(WidgetPosition position) {
	switch (position) {
        case WidgetPosition::QUARTER_TOP_LEFT:
		return "QUARTER_TOP_LEFT";
        case WidgetPosition::QUARTER_TOP_RIGHT:
		return "QUARTER_TOP_RIGHT";
        case WidgetPosition::QUARTER_BOTTOM_LEFT:
		return "QUARTER_BOTTOM_LEFT";
        case WidgetPosition::QUARTER_BOTTOM_RIGHT:
		return "QUARTER_BOTTOM_RIGHT";
        default:
		return "UNKNOWN";
	}
}

int Supervisor::registerWidget(std::shared_ptr<Widget> widget, WidgetPosition position) {
	m_widgetMap[position] = widget;
	BOOST_LOG_TRIVIAL(debug) << "Supervisor: " << widget->getName() << " registered at position: " << positionToString(position);
	return 0;
}

