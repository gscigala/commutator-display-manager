#ifndef WIDGETIDFMLINEREPORTS_H
#define WIDGETIDFMLINEREPORTS_H

#include "Widget.h"
#include "CommutatorIdfmLineReports.h"
#include "IdfmTransportMode.h"

#include <memory>
#include <string>

class WidgetIdfmLineReports: public Widget {
public:
	WidgetIdfmLineReports(std::shared_ptr<CommutatorIdfmLineReports> commutator, IdfmTransportMode mode, std::string name);

	std::string getName() const {
		std::string str ="Widget" + m_commutator->getName() + "(" + idfmTransportModeToString(m_mode) + " " + m_name + ")";
		return str;
	}

private:
	void newDataSlot(IdfmTransportMode mode, const std::string& name);

	const std::shared_ptr<CommutatorIdfmLineReports> m_commutator;

	const IdfmTransportMode m_mode;
	const std::string m_name;

	std::string m_severityEffect;
	std::string m_severityColor;
};

#endif // WIDGETIDFMLINEREPORTS_H
