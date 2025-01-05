#ifndef WIDGETSYTADIN_H
#define WIDGETSYTADIN_H

#include "CommutatorSytadin.h"
#include "Widget.h"

#include <memory>
#include <string>

class WidgetSytadin: public Widget {
public:
	WidgetSytadin(std::shared_ptr<CommutatorSytadin> commutator);

	std::string getName() const {
		return  "Widget" + m_commutator->getName();
	}

private:
	void newDataSlot();

	const std::shared_ptr<CommutatorSytadin> m_commutator;

	const int m_trafficValueThreshold;

	std::string m_trafficLevel;
	std::string m_trafficTendency;
	int m_trafficValue;
};

#endif // WIDGETSYTADIN_H
