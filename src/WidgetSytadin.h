#ifndef WIDGETSYTADIN_H
#define WIDGETSYTADIN_H

#include "Widget.h"
#include "CommutatorSytadin.h"

#include <memory>
#include <string>

class WidgetSytadin: public Widget {
public:
	WidgetSytadin(std::shared_ptr<CommutatorSytadin> commutator, std::string resDirPath);
	~WidgetSytadin();

	std::string getName() const {
		return  "Widget" + m_commutator->getName();
	}

private:
	void newDataSlot();
	void redraw();

	const std::shared_ptr<CommutatorSytadin> m_commutator;

	const int m_trafficValueThreshold;

	const int m_displayLine1RowsNb;
	const int m_displayLine1OffsetY;

	const int m_onlyTrafficValueThreshold;
	bool m_onlyTrafficValue;

	std::string m_trafficLevel;
	std::string m_trafficTendency;
	int m_trafficValue;

	Display::Color** m_carImage;
	Display::Color** m_arrowTopRightImage;
	Display::Color** m_arrowRightImage;
	Display::Color** m_arrowBottomRightImage;
};

#endif // WIDGETSYTADIN_H
