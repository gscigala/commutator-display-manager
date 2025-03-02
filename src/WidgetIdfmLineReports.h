#ifndef WIDGETIDFMLINEREPORTS_H
#define WIDGETIDFMLINEREPORTS_H

#include "Widget.h"
#include "CommutatorIdfmLineReports.h"
#include "IdfmTransportMode.h"

#include <memory>
#include <string>

class WidgetIdfmLineReports: public Widget {
public:
	WidgetIdfmLineReports(std::shared_ptr<CommutatorIdfmLineReports> commutator, std::string resDirPath,  IdfmTransportMode mode, std::string name);
	~WidgetIdfmLineReports();

	std::string getName() const {
		std::string str ="Widget" + m_commutator->getName() + "(" + idfmTransportModeToString(m_mode) + " " + m_name + ")";
		return str;
	}

	static IdfmTransportMode stringToTransportMode(std::string str)	{
		if (str == "METRO")
			return IdfmTransportMode::METRO;
		else if (str == "RAPID_TRANSIT")
			return IdfmTransportMode::RAPID_TRANSIT;
		else if (str == "LOCAL_TRAIN")
			return IdfmTransportMode::LOCAL_TRAIN;
		else
			return IdfmTransportMode::UNKNOWN;
	}

private:
	void newDataSlot(IdfmTransportMode mode, const std::string& name);
	void redraw();

	std::string toLowerCase(const std::string& str);

	const std::shared_ptr<CommutatorIdfmLineReports> m_commutator;

	const int m_displayLineRowsNb;
	const int m_displayLineOffsetY;

	const IdfmTransportMode m_mode;
	const std::string m_name;

	std::string m_severityEffect;

	Display::Color** m_modeImage;
	Display::Color** m_nameImage;
	Display::Color** m_trafficWarningImage;
	Display::Color** m_trafficStoppedImage;
};

#endif // WIDGETIDFMLINEREPORTS_H
