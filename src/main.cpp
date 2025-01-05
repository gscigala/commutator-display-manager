#include "Supervisor.h"
#include "CommutatorSytadin.h"
#include "WidgetSytadin.h"
#include "CommutatorVigicrues.h"
#include "WidgetVigicrues.h"
#include "IdfmTransportMode.h"
#include "CommutatorIdfmLineReports.h"
#include "WidgetIdfmLineReports.h"

#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

int main() {
	Supervisor supervisor;

	std::shared_ptr<CommutatorSytadin> commutatorSytadin = std::make_shared<CommutatorSytadin>();
	std::shared_ptr<CommutatorVigicrues> commutatorVigicrues = std::make_shared<CommutatorVigicrues>();
	std::shared_ptr<CommutatorIdfmLineReports> commutatorIdfmLineReports = std::make_shared<CommutatorIdfmLineReports>();

	/* WIP rework to use program arguments to configure widgets */

	std::shared_ptr<WidgetSytadin> widget1;
	std::shared_ptr<WidgetVigicrues> widget2;
	std::shared_ptr<WidgetIdfmLineReports> widget3;
	std::shared_ptr<WidgetIdfmLineReports> widget4;
	
	try {
	        widget1 = std::make_shared<WidgetSytadin>(commutatorSytadin);
		widget2 = std::make_shared<WidgetVigicrues>(commutatorVigicrues, WidgetVigicrues::Capability::WATER_LEVEL);
		widget3 = std::make_shared<WidgetIdfmLineReports>(commutatorIdfmLineReports, IdfmTransportMode::RAPID_TRANSIT, "C");
		widget4 = std::make_shared<WidgetIdfmLineReports>(commutatorIdfmLineReports, IdfmTransportMode::LOCAL_TRAIN, "N");
	} catch (const std::invalid_argument& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	if (supervisor.registerWidget(widget1,
				      Supervisor::WidgetPosition::QUARTER_TOP_LEFT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widge1t!";
		return -1;
	}

	if (supervisor.registerWidget(widget2,
				      Supervisor::WidgetPosition::QUARTER_TOP_RIGHT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widget2!";
		return -1;
	}

	if (supervisor.registerWidget(widget3,
				      Supervisor::WidgetPosition::QUARTER_BOTTOM_LEFT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widget3!";
		return -1;
	}

	if (supervisor.registerWidget(widget4,
				      Supervisor::WidgetPosition::QUARTER_BOTTOM_RIGHT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widget4!";
		return -1;
	}

	/* WIP TODO handle SIGTERM, SIGINT... */
	while(1) {
		sleep(1);
	}

	return 0;
}
