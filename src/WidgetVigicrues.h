#ifndef WIDGETVIGICRUES_H
#define WIDGETVIGICRUES_H

#include "Widget.h"
#include "CommutatorVigicrues.h"

#include <memory>
#include <string>

class WidgetVigicrues: public Widget {
public:
	enum class Capability {
		FLOW,
		WATER_LEVEL
	};

	WidgetVigicrues(std::shared_ptr<CommutatorVigicrues> commutator, Capability capability);

	std::string getName() const {
		std::string str = "Widget" + m_commutator->getName() + "(" + capabilityToString(m_capability) + ")";
		return str;
	}

private:
	void newDataSlot();
	std::string capabilityToString(Capability capability) const;

	const std::shared_ptr<CommutatorVigicrues> m_commutator;
	
	Capability m_capability;

	std::string m_tendency;
	float m_value;
};

#endif // WIDGETVIGICRUES_H
