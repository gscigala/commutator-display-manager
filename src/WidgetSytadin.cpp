#include "WidgetSytadin.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

WidgetSytadin::WidgetSytadin(std::shared_ptr<CommutatorSytadin> commutator): Widget(), m_commutator(commutator), m_trafficValueThreshold(5) {

	m_trafficLevel = m_commutator->getTrafficLevel();
	m_trafficTendency = m_commutator->getTrafficTendency();
	m_trafficValue = m_commutator->getTrafficValue();
	BOOST_LOG_TRIVIAL(trace) << getName() << ": trafficLevel = " << m_trafficLevel << ", trafficTendency = " << m_trafficTendency << ", trafficValue = " << m_trafficValue;

	m_commutator->newData.connect(boost::bind(&WidgetSytadin::newDataSlot, this));
}

void WidgetSytadin::newDataSlot() {
	BOOST_LOG_TRIVIAL(debug) << "WidgetSytadin: new data!";

	std::string tmpTrafficLevel, tmpTrafficTendency;
	int tmpTrafficValue;
	bool update = false;

	tmpTrafficLevel = m_commutator->getTrafficLevel();
	if (tmpTrafficLevel != m_trafficLevel)
		update = true;

	tmpTrafficTendency = m_commutator->getTrafficTendency();
	if (tmpTrafficTendency != m_trafficTendency)
		update = true;

	tmpTrafficValue = m_commutator->getTrafficValue();
	if (abs(tmpTrafficValue - m_trafficValue) >= m_trafficValueThreshold)
		update = true;

	if (update) {
		BOOST_LOG_TRIVIAL(trace) << "WidgetSytadin: need update widget";

		m_trafficLevel = tmpTrafficLevel;
		m_trafficTendency = tmpTrafficTendency;
		m_trafficValue = tmpTrafficValue;

		/* WIP TODO build widget display here*/
	}
}
