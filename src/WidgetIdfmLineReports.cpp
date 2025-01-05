#include "WidgetIdfmLineReports.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

WidgetIdfmLineReports::WidgetIdfmLineReports(std::shared_ptr<CommutatorIdfmLineReports> commutator, IdfmTransportMode mode, std::string name): Widget(), m_commutator(commutator), m_mode(mode), m_name(name) {

	m_severityEffect = m_commutator->getLineSeverityEffect(m_mode, m_name);
	BOOST_LOG_TRIVIAL(trace) << getName() << ": m_severityEffect = " << m_severityEffect;
	
	m_commutator->newData.connect(boost::bind(&WidgetIdfmLineReports::newDataSlot, this, std::placeholders::_1, std::placeholders::_2));
}

void WidgetIdfmLineReports::newDataSlot(IdfmTransportMode mode, const std::string& name) {
	BOOST_LOG_TRIVIAL(debug) << getName() << ": new data from (" << idfmTransportModeToString(mode) << " " << name << ")";

	if ((mode == m_mode) && (name == m_name)) {
		std::string severityEffect = m_commutator->getLineSeverityEffect(m_mode, m_name);
		if (m_severityEffect != severityEffect) {
			BOOST_LOG_TRIVIAL(debug) << getName() << ": new data match with this widget!";
			m_severityEffect = severityEffect;

			/* WIP TODO build widget display here*/
		}
	}
}
