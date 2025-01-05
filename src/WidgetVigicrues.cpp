#include "WidgetVigicrues.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

WidgetVigicrues::WidgetVigicrues(std::shared_ptr<CommutatorVigicrues> commutator, Capability capability): Widget(), m_commutator(commutator), m_capability(capability) {

	if (m_capability == Capability::FLOW) {
		m_tendency = m_commutator->getFlowTendency();
		m_value = m_commutator->getFlowValue();
		BOOST_LOG_TRIVIAL(trace) << getName() << ": tendency = " << m_tendency << ", value = " << m_value;
	}
	else if (m_capability == Capability::WATER_LEVEL) {
		m_tendency = m_commutator->getWaterLevelTendency();
		m_value = m_commutator->getWaterLevelValue();
		BOOST_LOG_TRIVIAL(trace) << getName() << ": tendency = " << m_tendency << ", value = " << m_value;
	}
	else {
		throw std::out_of_range("Unknown capabiliy!");
	}
	
	m_commutator->newData.connect(boost::bind(&WidgetVigicrues::newDataSlot, this));
}

void WidgetVigicrues::newDataSlot() {
	BOOST_LOG_TRIVIAL(debug) << getName() << ": new data!";

	std::string tmpTendency;
	float tmpValue;

	if (m_capability == Capability::FLOW) {
		tmpTendency = m_commutator->getFlowTendency();
		tmpValue = m_commutator->getFlowValue();
	}
	else if (m_capability == Capability::WATER_LEVEL) {
		tmpTendency = m_commutator->getWaterLevelValue();
		tmpValue = m_commutator->getWaterLevelValue();
	}
	else {
		BOOST_LOG_TRIVIAL(error) << getName() << ": Unknown capability!";
	}

	if ((m_value != tmpValue) || (m_tendency != tmpTendency)) {
		BOOST_LOG_TRIVIAL(trace) << getName() << ": need update widget";

		m_tendency = tmpTendency;
		m_value = tmpValue;

		/* WIP TODO build widget display here*/
	}
}

std::string WidgetVigicrues::capabilityToString(Capability capability) const {

	std::string str;

	switch(capability) {
	case (Capability::FLOW): {
		str = "FLOW";
		break;
	}
	case (Capability::WATER_LEVEL): {
		str= "WATER_LEVEL";
		break;
	}
	default: {
		str = "UNKNOWN";
		break;
	}
	}

	return str;
}
