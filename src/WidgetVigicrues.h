#ifndef WIDGETVIGICRUES_H
#define WIDGETVIGICRUES_H

#include "Widget.h"
#include "CommutatorVigicrues.h"

#include <memory>
#include <string>
#include <cmath>

class WidgetVigicrues: public Widget {
public:
	enum class Capability {
		FLOW,
		WATER_LEVEL,
		UNKNOWN
	};

	WidgetVigicrues(std::shared_ptr<CommutatorVigicrues> commutator, std::string resDirPath,  Capability capability, float alertThreshold, float floodThreshold);
	~WidgetVigicrues();

	std::string getName() const {
		std::string str = "Widget" + m_commutator->getName() + "(" + capabilityToString(m_capability) + ")";
		return str;
	}

	static Capability stringToCapability(std::string str) {
		if (str == "FLOW")
			return Capability::FLOW;
		else if (str == "WATER_LEVEL")
			return Capability::WATER_LEVEL;
		else
			return Capability::UNKNOWN;
	}

private:
	void newDataSlot();
	void redraw();
	std::string capabilityToString(Capability capability) const;

	bool areEqual(double a, double b) const {
		return std::fabs(a - b) < 1e-9;
	}

	const std::shared_ptr<CommutatorVigicrues> m_commutator;

	const float m_alertThreshold;
	const float m_floodThreshold;

	const int m_displayLine1RowsNb;
	const int m_displayLine1OffsetY;
	
	Capability m_capability;

	std::string m_tendency;
	float m_value;

	Display::Color** m_waveImage;
	Display::Color** m_symbolImage;
	Display::Color** m_arrowTopRightImage;
	Display::Color** m_arrowRightImage;
	Display::Color** m_arrowBottomRightImage;
};

#endif // WIDGETVIGICRUES_H
