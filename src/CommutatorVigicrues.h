#ifndef COMMUTATORVIGICRUES_H
#define COMMUTATORVIGICRUES_H

#include "Commutator.h"

#include <sdbus-c++/sdbus-c++.h>
#include <boost/signals2.hpp>

class CommutatorVigicrues: public Commutator {
public:

	CommutatorVigicrues();

	std::string getFlowTendency() const {
		return m_flowTendency;
	}

	float getFlowValue() const {
		return m_flowValue;
	}

	std::string getWaterLevelTendency() const {
		return m_waterLevelTendency;
	}

	float getWaterLevelValue() const {
		return m_waterLevelValue;
	}

	boost::signals2::signal<void()> newData;

private:
	void onPropertiesChanged(sdbus::Signal& signal);

	std::shared_ptr<sdbus::IProxy> m_proxy;

	std::string m_flowTendency;
	float m_flowValue;
	std::string m_waterLevelTendency;
	float m_waterLevelValue;
};

#endif // COMMUTATORVIGICRUES_H
