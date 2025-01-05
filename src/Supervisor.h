#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include "Widget.h"

#include <map>
#include <memory>

class Supervisor {
public:
	enum class WidgetPosition {
		QUARTER_TOP_LEFT,
		QUARTER_TOP_RIGHT,
		QUARTER_BOTTOM_LEFT,
		QUARTER_BOTTOM_RIGHT
	};

	Supervisor();
	int registerWidget(std::shared_ptr<Widget> widget, WidgetPosition position);

private:
	std::string positionToString(WidgetPosition position);

	std::map<WidgetPosition, std::shared_ptr<Widget>> m_widgetMap;
};

#endif // SUPERVISOR_H
